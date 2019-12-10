// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_GRPCONTAINER_H
#define MUMBLE_MURMUR_GRPCONTAINER_H

#include <boost/mp11.hpp>
#include <boost/mp11/mpl.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <boost/range.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/fiber/all.hpp>

#include <tuple>
#include <type_traits>
#include <memory>
#include <mutex>

namespace MurmurRPC {
	namespace Wrapper {
		namespace Container {

			struct server {};
			struct rpcid {};
			struct serveraction {};

			namespace mi = boost::multi_index;

			template<typename T>
			struct mapConfig {
				typedef std::tuple<int, uint32_t, std::weak_ptr<T>> value_type;
				typedef std::tuple<int, uint32_t, std::shared_ptr<T>> locked_type;

				using indices = std::tuple<
						mi::ordered_unique<
							mi::tag<server>,
							mi::global_fun<
								const value_type&,
								const int&,
								static_cast<const int& (*)(const value_type&)>(std::get<0>)
							>
						>,
						mi::ordered_unique<
							mi::tag<rpcid>,
							mi::global_fun<
								const value_type&,
								const uint32_t&,
								static_cast<const uint32_t& (*)(const value_type&)>(std::get<1>)
							>
						>
					>;
			};

			template<typename T>
			struct multiMapConfig {
				typedef std::tuple<int, uint32_t, std::weak_ptr<T>> value_type;
				typedef std::tuple<int, uint32_t, std::shared_ptr<T>> locked_type;

				using indices = std::tuple<
						mi::ordered_non_unique<
							mi::tag<server>,
							mi::global_fun<
								const value_type&,
								const int&,
								static_cast<const int& (*)(const value_type&)>(std::get<0>)
							>
						>,
						mi::ordered_non_unique<
							mi::tag<rpcid>,
							mi::global_fun<
								const value_type&,
								const uint32_t&,
								static_cast<const uint32_t& (*)(const value_type&)>(std::get<1>)
							>
						>
					>;
			};

			template<typename T>
			struct contextActionConfig {
				typedef std::tuple<int, std::string, uint32_t, std::weak_ptr<T>> value_type;
				typedef std::tuple<int, std::string, uint32_t, std::shared_ptr<T>> locked_type;

				using indices = std::tuple<
						mi::hashed_non_unique<
							mi::tag<serveraction>,
							mi::composite_key<
								value_type,
								mi::global_fun<
									const value_type&,
									const int&,
									static_cast<const int& (*)(const value_type&)>(std::get<0>)
								>,
								mi::global_fun<
									const value_type&,
									const std::string&,
									static_cast<const std::string& (*)(const value_type&)>(std::get<1>)
								>
							>
						>,
						mi::ordered_non_unique<
							mi::tag<rpcid>,
							mi::global_fun<
								const value_type &,
								const uint32_t&,
								static_cast<const uint32_t& (*)(const value_type&)>(std::get<2>)
							>
						>
					>;
			};


			template<typename T>
			struct setConfig {

				typedef std::tuple<uint32_t, std::weak_ptr<T>> value_type;
				typedef std::tuple<uint32_t, std::shared_ptr<T>> locked_type;

				using indices = std::tuple<
					mi::ordered_unique<
							mi::tag<rpcid>,
							mi::global_fun<
								const value_type&,
								const uint32_t&,
								static_cast<const uint32_t& (*)(const value_type&)>(std::get<0>)
							>
						>
					>;
			};

			template<typename C>
			class weakContainer {

			public:
				typedef typename C::value_type value_type;
				typedef typename C::locked_type locked_type;

				weakContainer() = default;
				~weakContainer() = default;
				weakContainer(const weakContainer&) = delete;
				weakContainer& operator=(const weakContainer&) = delete;

			private:
				using container_t = mi::multi_index_container<value_type, typename C::indices>;

				container_t container;
				boost::fibers::recursive_mutex m_Mtx;

				struct locked_adapter {
					typedef locked_type result_type;

					template<class ...T>
					locked_type operator()(std::tuple<T...> const & arg) const {
						static_assert(std::is_same<std::remove_cv_t<std::remove_reference_t<decltype(arg)>>, value_type>::value, "arg is not value_type!");
						namespace mp11 = boost::mp11;
						using L1 = std::tuple<T...>;
						using head_t = mp11::mp_pop_back<L1>;
						auto head = head_t();
						using head_idx = mp11::mp_iota<mp11::mp_size<head_t>>;
						mp11::mp_for_each<head_idx>([&head, &arg](auto i){
									std::get<i>(head) = std::get<i>(arg);
								});
						mp11::mp_back<locked_type> tail = (std::get<sizeof...(T) - 1>(arg)).lock();
					    return std::tuple_cat(std::move(head), std::make_tuple(tail));
					}
				};

				struct locked_filter {
					template<typename... types>
					bool operator()(const std::tuple<types...>& tup) const {
						static_assert(std::is_same<std::remove_cv_t<std::remove_reference_t<decltype(tup)>>, locked_type>::value, "arg is not locked_type!");
						return std::get<(sizeof...(types) - 1)>(tup) == nullptr;
					}
				};

				struct lock_holder{
					std::unique_lock<boost::fibers::recursive_mutex> lock;

					lock_holder(boost::fibers::recursive_mutex& mtx) : lock(mtx, std::defer_lock) {
						lock.lock();
					}

					lock_holder() = default;

					lock_holder(lock_holder&& other) noexcept {
						std::swap(this->lock, other.lock);
					}
					lock_holder& operator=(lock_holder&& other) noexcept {
						std::swap(this->lock, other.lock);
					}

					lock_holder(const lock_holder&) = delete;

					lock_holder& operator=(const lock_holder&) = delete;

					~lock_holder() noexcept {
						if (lock.owns_lock()) {
							lock.unlock();
						}
					}

					template<typename T>
					void operator()(T* t) {
						(void) t;
					}
				};

				template<typename Idx>
				typename std::unique_ptr<Idx, decltype(lock_holder())>
				makeUniqueIdx(Idx& index) {
					return std::unique_ptr<Idx, lock_holder>(std::addressof(index),
							lock_holder(this->m_Mtx));
				}

				template<typename It>
				auto getLockedRange(const std::pair<It, It>& rng);

			public:

				typedef typename container_t::iterator iterator;

				template<typename Functor>
				auto getLocked(Functor&& func);

				template<typename Idx>
				auto getLockedIndex(const Idx&);

				template<typename Q = server>
				auto getServerPtr() -> decltype(makeUniqueIdx(mi::get<Q>(container))) {

					return makeUniqueIdx(mi::get<Q>(container));
				}

				template<typename Q = server>
				auto getServerIndex() const -> decltype(mi::get<Q>(container)) {

					return mi::get<Q>(container);
				}

				template<typename Q = rpcid>
				auto getRPCIdPtr() -> decltype(makeUniqueIdx(mi::get<Q>(container))) {
					return makeUniqueIdx(mi::get<Q>(container));
				}

				template<typename Q = rpcid>
				auto getRPCIdIndex() const -> decltype(mi::get<Q>(container)) {
					return mi::get<Q>(container);
				}

				template<typename Q = serveraction>
				auto getActionPtr() -> decltype(makeUniqueIdx(mi::get<Q>(container))) {
					return makeUniqueIdx(mi::get<Q>(container));
				}

				template<typename Q = serveraction>
				auto getActionIndex() const -> decltype(mi::get<Q>(container)) {
					return mi::get<Q>(container);
				}

				template<typename ... Args>
				std::pair<iterator, bool> emplace(Args&& ... args) {
					auto lk = std::lock_guard(m_Mtx);
					return container.emplace(std::forward<Args>(args)...);
				}

			};
		} //end Container
	} // wrapper
}//MurmurRPC
#endif

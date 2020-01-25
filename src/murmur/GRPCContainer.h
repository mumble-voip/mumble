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
#include <boost/type_traits.hpp>

#include <tuple>
#include <cstddef>
#include <type_traits>
#include <memory>
#include <mutex>

namespace MurmurRPC {
	namespace Wrapper {
		namespace Container {

			namespace mi = boost::multi_index;
			namespace mp11 = boost::mp11;

			namespace Detail {
				template<typename Value, int idx>
				struct tuple_extractor {
					using const_lref_t = boost::add_lvalue_reference_t<
						boost::add_const_t<
							boost::remove_cv_ref_t<Value>
						>
					>;

					typedef decltype(std::get<idx>(const_lref_t{})) result_type;

					auto operator()(const_lref_t x) const -> decltype(std::get<idx>(x)) {
						return std::get<idx>(x);
					}
				};

				template<typename Adapter, typename Pred, typename It>
				struct range_type_it {
					using pred_t = boost::decay_t<Pred>;
					using adapter_t = boost::decay_t<Adapter>;
					using rng_t = boost::iterator_range<It>;
					using xformed_t = boost::transformed_range<adapter_t, rng_t>;
					using type = decltype(
						boost::remove_if<boost::return_begin_found>(
							std::declval<xformed_t>(),
							std::declval<adapter_t>()
							)
						);
				};

				template<typename Adapter, typename Pred, typename It>
					using range_type_it_t = typename range_type_it<Adapter, Pred, It>::type;

				template<typename Adapter, typename Pred, typename Idx>
				struct range_type_idx {
					using idx_t = boost::decay_t<Idx>;
					using It = typename idx_t::const_iterator;
					using type = range_type_it_t<Adapter, Pred, It>;
				};

				template<typename Adapter, typename Pred, typename Idx>
					using range_type_idx_t = typename range_type_idx<Adapter, Pred, Idx>::type;

				template<typename Adapter, typename Pred, typename Functor, typename Container>
				struct range_type_func {
					using container_lref_t = boost::add_lvalue_reference_t<
						boost::add_const_t<
							boost::decay_t<Container>
							>
						>;
					using pair_t = boost::decay_t<decltype(std::declval<Functor>()(container_lref_t{}))>;
					static_assert(mp11::mp_is_list<pair_t>::value, "func must have signature std::pair<Iter, Iter>(&Container)");
					using voids = mp11::mp_transform<mp11::mp_void, pair_t>;
					static_assert(std::is_same<voids, std::pair<void, void>>::value &&
						mp11::mp_same<pair_t>::value , "func must have signature std::pair<Iter, Iter>(&Container)");
					using type = range_type_it_t<Adapter, Pred, mp11::mp_front<pair_t>>;
				};

				template<typename Adapter, typename Pred, typename Functor, typename Container>
					using range_type_func_t = typename range_type_func<Adapter, Pred, Functor, Container>::type;
			} //end Detail

			struct server {};
			struct rpcid {};
			struct serveraction {};

			template<typename T>
			struct mapConfig {
				typedef std::tuple<int, uint32_t, std::weak_ptr<T>> value_type;
				typedef std::tuple<int, uint32_t, std::shared_ptr<T>> locked_type;

				using indices = std::tuple<
						mi::ordered_unique<
							mi::tag<server>,
							Detail::tuple_extractor<value_type, 0>
						>,
						mi::ordered_unique<
							mi::tag<rpcid>,
							Detail::tuple_extractor<value_type, 1>
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
							Detail::tuple_extractor<value_type, 0>
						>,
						mi::ordered_non_unique<
							mi::tag<rpcid>,
							Detail::tuple_extractor<value_type, 1>
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
								Detail::tuple_extractor<value_type, 0>,
								Detail::tuple_extractor<value_type, 1>
							>
						>,
						mi::ordered_non_unique<
							mi::tag<rpcid>,
							Detail::tuple_extractor<value_type, 2>
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
							Detail::tuple_extractor<value_type, 0>
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

					template<class Tp1, class Tp2, std::size_t... I>
					constexpr static Tp1 make_head(Tp2&& tp2, mp11::index_sequence<I...>) {
						return Tp1(std::get<I>(std::forward<Tp2>(tp2))...);
					}

					template<class ...T>
					locked_type operator()(std::tuple<T...> const & arg) const {
						static_assert(std::is_same<boost::remove_cv_t<boost::remove_reference_t<decltype(arg)>>, value_type>::value, "arg is not value_type!");
						namespace mp11 = boost::mp11;
						using L1 = std::tuple<T...>;
						using head_t = mp11::mp_pop_back<L1>;
						using head_idx = mp11::make_index_sequence<sizeof...(T) - 1>;
						auto head = make_head<head_t>(arg, head_idx{});
						mp11::mp_back<locked_type> tail = (std::get<sizeof...(T) - 1>(arg)).lock();
					    return std::tuple_cat(std::move(head), std::make_tuple(tail));
					}
				};

				struct locked_filter {
					template<typename... types>
					bool operator()(const std::tuple<types...>& tup) const {
						static_assert(std::is_same<boost::remove_cv_t<boost::remove_reference_t<decltype(tup)>>, locked_type>::value, "arg is not locked_type!");
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
					return std::unique_ptr<Idx, lock_holder>(
							std::addressof(index),
							lock_holder(this->m_Mtx)
						);
				}

				template<typename It>
				auto getLockedRange(const std::pair<It, It>& rng)
					-> Detail::range_type_it_t<
						decltype(locked_adapter()),
						decltype(locked_filter()), It
					>;

			public:

				typedef typename container_t::iterator iterator;

				template<typename Functor>
				auto getLocked(Functor&& func) -> Detail::range_type_func_t<
						decltype(locked_adapter()), decltype(locked_filter()),
						Functor, decltype(*this)>;

				template<typename Idx>
				auto getLockedIndex(const Idx&)
					-> Detail::range_type_idx_t<
						decltype(locked_adapter()),
						decltype(locked_filter()),
						decltype(mi::get<Idx>(container))
						>;

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
					std::lock_guard<decltype(m_Mtx)> lk(m_Mtx);
					return container.emplace(std::forward<Args>(args)...);
				}

			};
		} //end Container
	} // wrapper
}//MurmurRPC
#endif

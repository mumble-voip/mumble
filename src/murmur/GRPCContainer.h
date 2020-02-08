// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_GRPCONTAINER_H
#define MUMBLE_MURMUR_GRPCONTAINER_H

#include <QDebug>

#include <boost/container/slist.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/fiber/all.hpp>
#include <boost/mp11.hpp>
#include <boost/mp11/mpl.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/key_extractors.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/type_traits.hpp>

#include <cstddef>
#include <memory>
#include <mutex>
#include <tuple>
#include <type_traits>

namespace MurmurRPC {
	namespace Wrapper {
		namespace Container {

			namespace mi = boost::multi_index;
			namespace mp11 = boost::mp11;

			namespace Detail {
				/// \brief Helper key-extractor for a tuple.
				///
				/// Only exists because std::get<tuple> cannot be called as
				/// a global function in older versions of c++
				///
				/// \tparam Value typename of tuple to extract from
				/// \param idx index to be extracted
				template<typename Value, int idx>
				struct tuple_extractor {
					using const_lref_t = boost::add_lvalue_reference_t<
						boost::add_const_t<
							boost::remove_cv_ref_t<Value>
						>
					>;

					using result_type =decltype(std::get<idx>(const_lref_t{}));

					auto operator()(const_lref_t x) const -> decltype(std::get<idx>(x)) {
						return std::get<idx>(x);
					}
				};

				/* NOT USED ANYMORE, HOPEFULLY
				///
				/// helper template to determine the return type of
				/// weakContainer::getLockedRange()
				///
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

				///
				/// helper template to determine return type of
				/// weakContainer::getLockedIdx()
				///
				template<typename Adapter, typename Pred, typename Idx>
				struct range_type_idx {
					using idx_t = boost::decay_t<Idx>;
					using It = typename idx_t::const_iterator;
					using type = range_type_it_t<Adapter, Pred, It>;
				};

				template<typename Adapter, typename Pred, typename Idx>
					using range_type_idx_t = typename range_type_idx<Adapter, Pred, Idx>::type;

				///
				/// helper template to determine return type of
				/// weakContainer::getLocked()
				///
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
				*/
			} // namespace Detail

			struct server {}; ///< used in weakContainer as an index tag
			struct rpcid {}; ///< used in weakContainer as an index tag
			struct serveraction {}; ///< used in weakContainer as an index tag

			/// \brief Configuration template for weakContainer to resemble a std::map
			template<typename T>
			struct mapConfig {
				using value_type = std::tuple<int, uint32_t, std::weak_ptr<T>>;
				using locked_type = std::tuple<int, uint32_t, const std::shared_ptr<T>>;

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

			/// \brief Configuration template for weakContainer to resemble std::multimap
			template<typename T>
			struct multiMapConfig {
				using value_type = std::tuple<int, uint32_t, std::weak_ptr<T>>;
				using locked_type = std::tuple<int, uint32_t, const std::shared_ptr<T>>;

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

			/// \brief Configuration template for weakContainer used by contextActionEvents.
			///
			/// Resembles a std::unordered_map with the key being a std::pair<int, std::string>
			///
			template<typename T>
			struct contextActionConfig {
				using value_type = std::tuple<int, std::string, uint32_t, std::weak_ptr<T>>;
				using locked_type = std::tuple<int, std::string, uint32_t, const std::shared_ptr<T>>;

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


			/// \brief Configuration template for weakContainer to resemble std::set
			template<typename T>
			struct setConfig {

				using value_type = std::tuple<uint32_t, std::weak_ptr<T>>;
				using locked_type = std::tuple<uint32_t, const std::shared_ptr<T>>;

				using indices = std::tuple<
					mi::ordered_unique<
							mi::tag<rpcid>,
							Detail::tuple_extractor<value_type, 0>
						>
					>;
			};

			/// \brief Wrapper class for a boost::multi_index container.
			///
			/// Allows the container to be safely
			/// used across threads and without having to know many details about
			/// how boost::multi_index works.
			///
			/// Takes a configuration parameter as a template argument to generate
			/// the type of container we are going to emulate, as well as the
			/// actual values being stored in the container.
			///
			/// A typical configuration has is indexed on two items. One is the
			/// index that RPCCall users call in order to look up the std::weak_ptr
			/// that they need for a particular call. The second index is on
			/// the id of the RPCCall itself. This allows an RPCCall object to find
			/// itself in a container and remove itself when it is done. This would
			/// would be very hard to do without the second index as it would have
			/// to iterate through the entire container, and without the RPCId being
			/// stored, all it could look against would be a std::weak_ptr which is
			/// not a comparable object.
			///
			/// All the current configuration store std::tuples with the indexable
			/// items in the front, and the std::weak_ptr at the end. The current
			/// implementation assumes this layout in order to be able to generate
			/// 'safe' iterable ranges.
			///
			/// \tparam C configuration struct defining the container attributes
			template<typename C>
			class weakContainer {

			public:
				using value_type = typename C::value_type; ///< typename for what is being stored

				/// \brief typename for a modified tuple that has a std::shared_ptr at the end instead
				/// of a std::weak_ptr
				using locked_type = typename C::locked_type;

				weakContainer() = default; ///< defaulted constructor
				~weakContainer() = default; ///< defaulted deleteter
				weakContainer(const weakContainer&) = delete; ///< non-copyable
				weakContainer& operator=(const weakContainer&) = delete; ///< non-copyable
				weakContainer( weakContainer&&) = delete; ///< non-moveable
				weakContainer& operator=( weakContainer&&) = delete; ///< non-moveable

			private:
				using container_t = mi::multi_index_container<value_type, typename C::indices>;

				container_t container;

				///
				/// mutex used to prevent data races between fibers and threads using the
				/// container. It is recursive because many methods force this mutex
				/// to be held and it would be very easy to deadlock
				///
				boost::fibers::recursive_mutex m_Mtx;

				/// \brief helper class to 'lock' tuples.
				///
				/// It takes in a tuple
				/// stored by the container, copies everything except the end. It then calls
				/// lock() on the std::weak_ptr, resulting in a std::shared_ptr to the RPCCall
				/// or a std::shared_ptr holding nullptr. Then it puts it back together,
				/// resulting in a tuple just like the original except now that it holds
				/// a std::shared_ptr the RPCCall object cannot be deleted while iterating.
				///
				/// Thank boost::mp11 for this. Without it it would be near impossible to
				/// make this happen with one struct that works for every single container.
				///
				struct locked_adapter {
					using result_type = locked_type;

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

				/// \brief helper class used to filter for stale pointers
				///
				/// Filters a range of 'locked'
				/// types so that the ones where the std::shared_ptr is nullptr are removed
				///
				struct locked_filter {
					template<typename... types>
					bool operator()(const std::tuple<types...>& tup) const {
						static_assert(std::is_same<boost::remove_cv_t<boost::remove_reference_t<decltype(tup)>>, locked_type>::value, "arg is not locked_type!");
						return std::get<(sizeof...(types) - 1)>(tup) == nullptr;
					}
				};

				/// \brief Helper class used as a 'deleter' when accessing the container.
				///
				/// Non-const indices are only given out as std::unique_ptr with this deleter, which
				/// means that having any access to modify the container forces you to
				/// hold the lock.
				///
				/// By hiding the mutex lock this way it can be a private member variable.
				///
				struct lock_holder{

				private:
					std::unique_lock<boost::fibers::recursive_mutex> lock;

				public:
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

				///
				/// takes an lref to an index in the container, and turns it into a
				/// std::unique_ptr with locked_filter as the deleter. While this could
				/// be used to hold onto the mutex forever, the nature of std::unique_ptr
				/// makes it very hard to do so without intent.
				///
				template<typename Idx>
				typename std::unique_ptr<Idx, decltype(lock_holder())>
				makeUniqueIdx(Idx& index) {
					return std::unique_ptr<Idx, lock_holder>(
							std::addressof(index),
							lock_holder(this->m_Mtx)
						);
				}

				///
				/// helper function to convert a pair of iterators into a
				/// list that can be safely iterated over 
				/// without holding the lock on the container.
				///
				/// Boost Range is no longer used as it wasn't properly filtering.
				///
				/// \return `boost::container::slist<locked_type>` a singly-linked list
				/// designed for forward iteration only.
				template<typename It>
				auto getLockedRange(const std::pair<It, It>& rng) -> boost::container::slist<locked_type>;

			public:

				using iterator = typename container_t::iterator;

				///
				/// uses a user-provided functor that returns a iterator pair
				/// from the container to make a 'safe' range that can be iterated
				/// over without either holding the mutex for the container or without
				/// having check for deleted RPCCalls.
				///
				/// \param func a functor that takes in a const lref to this container,
				/// and returns an iterator pair of the range it wants to go over.
				///
				/// \return `boost::container::slist<locked_type>` with the locked values. This
				/// list is singly-linked, for forward iteration only.
				///
				template<typename Functor>
				auto getLocked(Functor&& func) -> boost::container::slist<locked_type>;

				///
				/// gets a 'safe' range for an index on this container which
				/// can be iterated over without any invalidation of the iterators and
				/// all RPCCall pointers are non-nullptr std::shared_ptr
				///
				/// \param Idx a reference to the 'tag' of the index you wish to get
				///
				/// \return `boost::container::slist<locked_type>` a singly-linked
				/// list with copies of the values you wanted.
				///
				template<typename Idx>
				auto getLockedIndex(const Idx&) -> boost::container::slist<locked_type>;

				/// \brief Only available if the container has an index on a serverId.
				///
				/// This will get the container mutex, then return to you a smart
				/// pointer that you can use as if this was a std::map or std::multimap
				/// depending on configuration.
				///
				/// \return a std::unique_ptr to that has an interface as if it
				/// was a standard container.
				///
				template<typename Q = server>
				auto getServerPtr() -> decltype(makeUniqueIdx(mi::get<Q>(container))) {

					return makeUniqueIdx(mi::get<Q>(container));
				}

				/// \brief Only available if container has index on a serverId.
				///
				/// \return const lreference that can be used like a standard container
				///
				template<typename Q = server>
				BOOST_ATTRIBUTE_NODISCARD auto getServerIndex() const -> decltype(mi::get<Q>(container)) {

					return mi::get<Q>(container);
				}

				/// \brief Available in all configurations.
				///
				/// Locks the container mutex then returns a smart pointer.
				///
				/// \return std::unique_ptr that can be used as if it was pointing to a std::set
				/// or std::multiset depending on configuration
				///
				template<typename Q = rpcid>
				auto getRPCIdPtr() -> decltype(makeUniqueIdx(mi::get<Q>(container))) {
					return makeUniqueIdx(mi::get<Q>(container));
				}

				/// Available in all configurations.
				///
				/// Gets a const lreference the rpcId index.
				///
				/// \return const lreference that acts like std::set or std::multiset
				///
				template<typename Q = rpcid>
				auto getRPCIdIndex() const -> decltype(mi::get<Q>(container)) {
					return mi::get<Q>(container);
				}

				/// Only for contextAction configurations.
				///
				/// Locks the container mutex, and returns a smart pointer that has
				/// an interface like a std::unordered_map keyed on
				/// std::pair<int, std::string> (serverId and the name of the contextaction)
				///
				/// \return std::unique_ptr with a std::unordered_map like interface
				///
				template<typename Q = serveraction>
				auto getActionPtr() -> decltype(makeUniqueIdx(mi::get<Q>(container))) {
					return makeUniqueIdx(mi::get<Q>(container));
				}

				/// \brief Only for contextAction configurations.
				///
				/// \return const lreference to what appears to be a std::unordered_map
				///
				template<typename Q = serveraction>
				BOOST_ATTRIBUTE_NODISCARD auto getActionIndex() const -> decltype(mi::get<Q>(container)) {
					return mi::get<Q>(container);
				}

				/// \brief Used to put new items into the container.
				///
				/// While new items can be inserted through the indices this is the way most
				/// items are expected to be inserted. It will hold the mutex during
				/// insertion.
				///
				/// \return std::pair<iterator, bool> where iterator is iterator
				/// deferencable to the item inserted, and bool indicates success or failure
				/// (if you tried to insert a duplicate item into a map/set-like index)
				///
				template<typename ... Args>
				std::pair<iterator, bool> emplace(Args&& ... args) {
					std::lock_guard<decltype(m_Mtx)> lk(m_Mtx);
					return container.emplace(std::forward<Args>(args)...);
				}

			};
		} // namespace Container
	} // namespace Wrapper
} // namespace MurmurRPC
#endif

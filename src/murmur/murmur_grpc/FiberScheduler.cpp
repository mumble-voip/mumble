// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "FiberScheduler.h"

namespace MurmurRPC {
namespace Scheduler {

namespace bf = boost::fibers;

grpc_scheduler::xferqueue_t grpc_scheduler::xferQueue{};
std::mutex grpc_scheduler::m_xferMtx{};
std::function<void()> grpc_scheduler::alertMain = [](){ return; };

grpc_scheduler::grpc_scheduler(bool is_event_loop) : is_loop_thread(is_event_loop) {
	if (is_event_loop) {
		alertMain = [&](){
			this->notify();
			auto dispatch = QCoreApplication::eventDispatcher();
			if (dispatch != nullptr) {
				dispatch->interrupt();
			}
		};
	}
}

void grpc_scheduler::awakened(bf::context* ctx, grpc_props& props) noexcept {
	if (ctx->is_context(bf::type::pinned_context) || !props.run_in_main()) {
		m_readyQueue.push_back(*ctx);
	} else if (props.run_in_main()) {
		if( this->is_loop_thread ) {
			m_readyQueue.push_back(*ctx);
		} else {
			ctx->detach();
			std::unique_lock<std::mutex> lk(m_xferMtx);
			xferQueue.push_back(ctx);
		}
		// make sure to wait up the event loop fiber
		// and get into a yield() call ASAP
		alertMain();
	}
}

bool grpc_scheduler::has_ready_fibers() const noexcept {
	if (this->is_loop_thread) {
		std::unique_lock<std::mutex> lk(m_xferMtx);
		return !xferQueue.empty() || !m_readyQueue.empty();
	}
	return !m_readyQueue.empty();
}

bf::context* grpc_scheduler::pick_next() noexcept {
	bf::context* ctx = nullptr;

	if (this->is_loop_thread) {
		std::unique_lock<std::mutex> lk(m_xferMtx);
		if (! xferQueue.empty()) {
			ctx = xferQueue.front();
			xferQueue.pop_front();
			lk.unlock();
			bf::context::active()->attach(ctx);
			return ctx;
		}
	}
	if (!m_readyQueue.empty()) {
		ctx = & m_readyQueue.front();
		m_readyQueue.pop_front();
	}
	return ctx;
}

void grpc_scheduler::suspend_until(std::chrono::steady_clock::time_point const& time_point) noexcept {
	if ((std::chrono::steady_clock::time_point::max)() == time_point) {
		std::unique_lock< std::mutex > lk{ m_rqMtx };
		m_cnd.wait(lk, [this](){ return flag; });
		flag = false;
	} else {
		std::unique_lock< std::mutex > lk{ m_rqMtx };
		m_cnd.wait_until(lk, time_point, [this](){ return flag; });
		flag = false;
	}
}

void grpc_scheduler::notify() noexcept {
	std::unique_lock< std::mutex > lk{ m_rqMtx };
	flag = true;
	lk.unlock();
	m_cnd.notify_all();
}

void grpc_scheduler::property_change(bf::context* ctx, grpc_props& props ) noexcept {

	// loop thread doesn't care if it does 'other' threads work
	if (this->is_loop_thread) {
		return;
	}
	// not ours? not a problem
	if (! ctx->ready_is_linked()) {
		return;
	}

	ctx->ready_unlink();
	awakened (ctx, props);
}

}}

// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may
// not use these files except in compliance with the License. You may obtain
// a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

#pragma once

namespace ABI { namespace Microsoft { namespace Graphics { namespace Canvas { namespace UI { namespace Xaml
{
    class CanvasAnimatedControl;

    class CanvasGameLoop : public std::enable_shared_from_this<CanvasGameLoop>,
                           private LifespanTracker<CanvasGameLoop>
    {
        std::mutex m_mutex;

        ComPtr<IAsyncAction> m_threadAction;
        ComPtr<ICoreDispatcher> m_dispatcher;

        // This is set by StartTickLoop, and is Reset when the tick loop decides
        // that it has finished running.
        ComPtr<IAsyncAction> m_tickLoopAction;

        // This is set when the tick loop finishes running and is used to query
        // the status (ie to marshal exceptions from the tick loop to the ui
        // thread).  It is reset by StartTickLoop.
        ComPtr<IAsyncInfo> m_completedTickLoopInfo;
        
        ComPtr<IDispatchedHandler> m_tickHandler;
        ComPtr<IAsyncActionCompletedHandler> m_tickCompletedHandler;
        bool m_tickLoopShouldContinue;

    public:
        CanvasGameLoop(ComPtr<IAsyncAction>&& action, ComPtr<ICoreDispatcher>&& dispatcher);

        ~CanvasGameLoop();

        void StartTickLoop(
            CanvasAnimatedControl* control,
            std::function<bool(CanvasAnimatedControl*)> const& tickFn,
            std::function<void(CanvasAnimatedControl*)> const& completedFn);

        void TakeTickLoopState(bool* isRunning, ComPtr<IAsyncInfo>* completedTickLoopState);

        bool HasThreadAccess();

    private:
        void ScheduleTick(Lock const& lock);
        void EndTickLoop(Lock const& lock);
    };

} } } } } }
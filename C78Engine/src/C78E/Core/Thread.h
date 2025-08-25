#pragma once
#include <C78E/Utils/StdUtils.h>

namespace C78E {

	using Thread = std::thread;

	class CallableThread {
	public:
		using Callable = std::function<bool()>; // Worker Function to be executed on the CallableThread, ensure its Halting(Turing Nightmare Flashbacks)
	protected:
		using OwnerID = std::thread::id;
	protected:
		CallableThread()
			: m_Owner(std::this_thread::get_id()) {
		}
		~CallableThread() {
			//TODO: cleanup all Tasks - check - other Threads are possibly waiting, so they cant die if CallableThread does not clean up
		}
	public:
		bool call(Callable function, const bool waitToFinish = true) {
			// if called from the same thread - just execute it now
			const OwnerID callerID = std::this_thread::get_id();
			if (callerID == m_Owner) {
				return function();
			}

			// Read Lock on Access -> everyone can read m_CurrentRecordContainer
			m_Access.lock_shared();
			TaskContainer& recordContainer = m_TaskContainer[m_CurrentRecordContainer];

			// Write Lock on addLock -> adding a Task requires extending the recording TaskContainer, writing
			recordContainer.addTask.lock();
			std::atomic<bool> taskSuccess = false;
			std::atomic<bool> taskFinished = false;
			recordContainer.tasks.emplace_back(
				function,
				&taskSuccess,
				waitToFinish ? &taskFinished : nullptr // nullptr indicates that the Thread is not waiting
			);
			recordContainer.addTask.unlock(); // Task appending finished - other threads can now append
			m_Access.unlock_shared(); // now also allow CallableThread to execute

			// opt. wait for the provided bool to get marked as finished - written to from CallableThread only
			if (waitToFinish) {
				taskFinished.wait(true);
			}

			return taskSuccess;
		}
	protected:
		struct Task {
			Callable callable;
			std::atomic<bool>* success = nullptr;
			std::atomic<bool>* finished = nullptr;
		};
		struct TaskContainer {
			std::shared_mutex addTask;
			std::vector<Task> tasks;
		};
	protected:
		bool validateOwner() const {
			return std::this_thread::get_id() == m_Owner;
		}

		void executeTasks() {
			// once the function is called the owner is verified - no guards needed
			// This system breaks horrificly when executeTasks is called from multiple Tasks
			C78E_CORE_VALIDATE(validateOwner(), return, "CallableThread::executeTasks: Was not called by owner!");
			
			// Swapping the ShadowIndex - exclusivly lock 
			m_Access.lock(); // TODO: currently deadlocks
			const bool executeContainerID = m_CurrentRecordContainer; // execute the current recorded Container
			m_CurrentRecordContainer = !m_CurrentRecordContainer; // Swap the ShadowIndex
			m_Access.unlock(); // Index is swapped - Callers can record to the new Container

			// Execute all recorded Tasks in order - no locking required, only CallableThread accesses them now
			TaskContainer& executeContainer = m_TaskContainer[executeContainerID];
			for (Task& task : executeContainer.tasks) {
				const bool success = task.callable(); // run the Task
				task.success->store(success); // return value
				if (task.finished) {
					task.finished->store(true); // mark the Task as done
				}
			}
			executeContainer.tasks.clear();
		}

	private:
		const OwnerID m_Owner; // ensure one owner thread
		std::shared_mutex m_Access; // Read / Write Lock to access m_CurrentRecordContainer
		bool m_CurrentRecordContainer = 0; // ShadowStorageIndex - one TaskContainer is recorded to one is getting executed
		TaskContainer m_TaskContainer[2]; // DoubleBuffered ShadowStorage
	};

}

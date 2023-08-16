#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

using namespace std::chrono_literals;

namespace jinx
{
    template <class T>
    class Safe_Q;

    template <class T>
    class Safe_Thread final
    {
        public:    

        // Public Methods    

        /*
        помещает в очередь очередную задачу. 
        В качестве принимаемого аргумента метод 
        может принимать или объект шаблона std::function, 
        или объект шаблона package_task
        */
        template <typename Function>
        void submit(Function task)
        {
            que.push(task);
        } // !submit()

        // Constructor
        Safe_Thread()
        {
            auto cores_awail = std::thread::hardware_concurrency();
            finish = false;
		
            for (unsigned i = 0; i < (cores_awail - 1); ++i)
			{
				runner.push_back(std::jthread(&Safe_Thread::work, this));
			}
        }

        // Destructor
        ~Safe_Thread()
        {
            finish = true;
        }            

        private:

        // Private Variables
        std::vector<std::jthread> runner;
        jinx::Safe_Q<T> que;  
        std::atomic_bool finish;

        // Private Methods

        /*
        Выбирает из очереди очередную задачи и 
        исполняет ее. Данный метод передается 
        конструктору потоков для исполнения
        */
        void work()
        {
            while (!finish)
		    {
			    T task;

			    if (!que.empty())
			    {
				    task = que.pop();
                    task();
				}
                else
			    {
				    std::this_thread::yield();
			    }			
			}			
		} // !work()

    }; // --- !class Safe_Thread ---

    template <class T>
    class Safe_Q final
    {
        public:

        /*
        Записывает в начало очереди новую задачу. 
        При этом захватывает мьютекс, а после окончания 
        операции нотифицируется условная переменная.
        */
        void push(T task)
        {
            // lock que
            std::unique_lock<std::mutex> lock(t_mute);
  
            // push new task
            task_que.push(task);
  
            // turn on one thread
            t_switch.notify_one();
        } // !push()

        /*
        находится в ожидании, пока не придут уведомление 
        на условную переменную. При нотификации условной 
        переменной данные считываются из очереди.
        */
        T pop()
        {
            // lock que
            std::unique_lock<std::mutex> lock(t_mute);
  
            // wait fot tasks in que
            t_switch.wait(lock, [this]() { return !task_que.empty(); });
  
            // get task from q
            T task = task_que.front();
            task_que.pop();
  
            // send
            return task;
        } // !pop()

        /*
        Проверка есть ли в очереди элементы
        */
        bool empty()
        {
            // lock que
            std::unique_lock<std::mutex> lock(t_mute);

            // get info
            return task_que.empty();
        }

        private:

        // Private variables 

        std::queue<T> task_que;
        std::mutex t_mute;
        std::condition_variable t_switch;
    }; // --- !class Safe_Q ---

} // !namespace jinx
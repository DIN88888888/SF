#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <array>
#include <thread>
#include <chrono>

using namespace std;

struct Node
{
    Node() { value = 0; next = nullptr; node_mutex = new mutex(); };
    Node(int val) { value = val; next = nullptr; node_mutex = new mutex(); };

    ~Node() { 
        delete node_mutex;
    }
    int value;
    Node* next;
    std::mutex* node_mutex;
};

class FineGrainedQueue
{    
    Node* head;
    std::mutex* queue_mutex;
public:
    FineGrainedQueue() { head = new Node(); queue_mutex = new mutex();}
    ~FineGrainedQueue() { //не предполагается потокобезопасность
        delete queue_mutex; 
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr;
    }
    void insertIntoMiddle(int value, int pos);
    int size() {//не предполагается потокобезопасность
        int i = 0;
        Node* cur = head;
        while (cur) { cur = cur->next; i++; }
        return i;
    }
};

void FineGrainedQueue::insertIntoMiddle(int value, int pos) {
    Node* NewNode = new Node(value);

    queue_mutex->lock();//блокируем очередь
        Node* prew = head;        
        prew->node_mutex->lock();//лочим голову

        Node* cur = nullptr;

    queue_mutex->unlock();//отпускаем очередь

    int n = 1;//ну мы уже типа на 0ом элементе, так что как буд-то на первом
    while (prew->next && n < pos) {        //идём вперёд пока есть куда и надо
        prew->next->node_mutex->lock();//блокируем то куда переходим, ссылка безопастна, так как prew залочен
        cur = prew->next;//сохраняем адрес куда идём
        prew->node_mutex->unlock();//освобождаем prew элемент
        prew = cur;//продолжаем работаеть с текущим элементом
        n++;
    }
    //на выходе prew - последний элемент списка или тот после которого нужно вставить
    // заблокированный нами
    NewNode->next = prew->next;//нет необходимости блокировать этот элемент, так как его не увидят до разблокировки prew
    prew->next = NewNode;
    prew->node_mutex->unlock();//отпускаем текущий элемент
}

//===============  MultiThTest ====================
#define TH_MAX 20//потоков
#define TH_AllValues 1000000//всего вставок
#define TH_Values (TH_AllValues / TH_MAX)//вставок на поток
#define TH_ExpectedLength (TH_Values * TH_MAX+1)//ожидаемое количество элементов в конце работы

thread* Workers[TH_MAX];
thread* Starters[TH_MAX];

shared_mutex GO;//симулируем одновременный старт... на сколько это возможно
void WorkerFunc(FineGrainedQueue& qwe, unsigned int rn, int thn) {
    srand(rn);
    for (int i = 0; i < TH_Values; i++) {
        qwe.insertIntoMiddle(i,abs(rand()) % TH_AllValues + 1);
    }
    cout << "thread " << thn << " stoped\n";//знаю что не хорошо, но для текущей задачи норм.
}
void StarterFunc(FineGrainedQueue &qwe,int thn) {    
    unsigned int rn = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    GO.lock_shared();
    Workers[thn] = new thread(WorkerFunc, std::ref(qwe), rn,thn);    
    GO.unlock_shared();
}



int main()
{
    {
        GO.lock();
        FineGrainedQueue qwe;
        for (int i = 0; i < TH_MAX; i++) {
            this_thread::sleep_for(10ms);
            Starters[i] = new thread(StarterFunc, std::ref(qwe), i);
        }
        this_thread::sleep_for(1000ms);
        GO.unlock();

        for (int i = 0; i < TH_MAX; i++) {
            Starters[i]->join();
        }
        for (int i = 0; i < TH_MAX; i++) {
            Workers[i]->join();
        }
        cout << "expected = " << TH_ExpectedLength << " real = " << qwe.size() << "\n\n\n";
        for (int i = 0; i < TH_MAX; i++) {
            delete Starters[i];
            delete Workers[i];
        }
    }
    int memtest;
    cin >> memtest;
}



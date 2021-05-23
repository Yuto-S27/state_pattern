#include <iostream>
#include <thread>

// 状態の宣言
enum class SCENE_STATE
{
    CREDIT = 0,
    TITLE,
    OPENING,
    DEMO,

    STATE_MAX,// 要素数の自動計算
};

// 基底クラス
class state
{
public:

    state() {}
    virtual ~state() {}

    virtual void setup() = 0;      // 初期化関数
    virtual SCENE_STATE update() = 0;   // 返り値は次の状態（自分の状態を返すと続けて呼ばれる）
};

// 具象クラス達
class state_credit : public state
{
public:
    state_credit() {}
    ~state_credit() {}

    void setup() {}

    SCENE_STATE update()
    {
        std::cout << "credit\n";

        std::this_thread::sleep_for(std::chrono::seconds(3));// 3 秒待つ

        return SCENE_STATE::TITLE;
    }
};

class state_title : public state
{
public:
    state_title() {}
    ~state_title() {}

    void setup() {}

    SCENE_STATE update()
    {
        std::cout << "title\n";

        std::this_thread::sleep_for(std::chrono::seconds(3));// 3 秒待つ

        return SCENE_STATE::OPENING;
    }
};

class state_opening : public state
{
public:
    state_opening() {}
    ~state_opening() {}

    void setup() {}

    SCENE_STATE update()
    {
        std::cout << "opening\n";

        std::this_thread::sleep_for(std::chrono::seconds(5));// 5 秒待つ

        return SCENE_STATE::DEMO;
    }
};

class state_demo : public state
{
public:
    state_demo() {}
    ~state_demo() {}

    void setup() {}

    SCENE_STATE update()
    {
        std::cout << "demo\n";

        std::this_thread::sleep_for(std::chrono::seconds(5));// 5 秒待つ

        return SCENE_STATE::CREDIT;
    }
};

class context
{
private:
    SCENE_STATE current_state;  // 現在の状態（比較用）
    state* current = nullptr;   // 現在の状態（呼び出し用） 
    state* a_state[(const int)SCENE_STATE::STATE_MAX];// 状態のインスタンスの保持

    // 状態からステートクラスを生成
    static state* generate(SCENE_STATE state)
    {
        switch (state){
        case SCENE_STATE::CREDIT:
            return new state_credit();
            break;
        case SCENE_STATE::TITLE:
            return new state_title();
            break;
        case  SCENE_STATE::OPENING:
            return new state_opening();
            break;
        case  SCENE_STATE::DEMO:
            return new state_demo();
            break;
        default:
            break;
        }
        return nullptr;
    }

public:
    context()
    {
        // 具象クラスを生成
        for (int i = 0; i < (int)SCENE_STATE::STATE_MAX; i++) {
            a_state[i] = generate((SCENE_STATE)i);
        }

        // 初期シーンの設定
        current_state = SCENE_STATE::CREDIT;
        current = a_state[(int)current_state];
        current->setup();
    }
    ~context()
    {
        // 確保したメモリの破棄
        for (int i = 0; i < (int)SCENE_STATE::STATE_MAX; i++) {
            delete  a_state[i];
        }
    }

    void update() 
    {
        // 現在の状態の実行
        SCENE_STATE s = current->update();

        // 状態の変更
        if (s != current_state) {
            current_state = s;
            current = a_state[(int)current_state];
            current->setup();
        }
    }
};

// メイン関数
int main()
{
    context ctx;

    while (true)
    {
        ctx.update();
    }
}


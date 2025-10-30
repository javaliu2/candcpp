class Useless {
    private:
        int n;  // 字符个数
        char* pc;
        static int ct;  // 对象个数统计
        void ShowObject() const;
    public:
        Useless();
        explicit Useless(int k);  // explicit禁止了隐式类型转换
        Useless(int k, char ch);
        Useless(const Useless& f);  // 拷贝构造函数
        Useless(Useless&& f);  // 移动构造函数
        ~Useless();
        Useless operator+(const Useless& f) const;
        Useless& operator=(const Useless& f);  // 左值引用赋值
        Useless& operator=(Useless&& f);  // 右值引用赋值
        void ShowData() const;
};
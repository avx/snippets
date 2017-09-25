#pragma GCC push_options
#pragma GCC optimize ("O3")

int func1() {
    int a=1;
    int b=2;

    return a+b;
}

#pragma GCC pop_options

int __attribute__((optimize("O0"))) func2() {
    int a=1;
    return a;
}

int main() {
    int i = 1;

    func1();

    return i;
}

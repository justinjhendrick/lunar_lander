extern void test_utils();
extern void test_vector();
extern void test_pilot(unsigned int n_trials);

int main() {
    test_utils();
    test_vector();
    test_pilot(100);
    return 0;
}

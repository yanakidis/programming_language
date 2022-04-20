program {
    int fib1 = 1;
    int fib2 = 1;
    int fibsum;
    int n;
    int i = 0;
    write("Number: ");
    read(n);
    while (i + n < 2){
        fibsum = fib1 + fib2;
        fib1 = fib2;
        fib2 = fibsum;
        i = i + 1;
    }
    write("Value: ", fib2);
    write(n , i , fibsum);
}

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <signal.h>
#include <time.h>

// Initialize variables.
mpz_t zero, one, two, seven, eight, exponent, exponent2, bigNumber, start, check, mod;
int ctrl_c_count = 0;
time_t last_ctrl_c_time = 0;

static void sig_handler(int _)
{
    ctrl_c_count++;
    gmp_printf("\nCurrent : %Zd\n", start);
    FILE *fptr;
    char buffer[32]; // The filename buffer.
    snprintf(buffer, sizeof(char) * 32, "%s.res", mpz_get_str(buffer, 10, exponent));
    fptr = fopen(buffer, "w");
    gmp_fprintf(fptr, "%Zd", start);
    fclose(fptr);
    if (ctrl_c_count >= 2 && time(NULL) - last_ctrl_c_time <= 5)
    {
        printf("Exiting program...\n");
        exit(0);
    }
    else
    {
        last_ctrl_c_time = time(NULL);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_handler);

    mpz_inits(zero, one, two, seven, eight, exponent, exponent2, bigNumber, start, check, mod, (mpz_ptr)0);
    mpz_set_ui(one, 1);
    mpz_set_ui(two, 2);
    mpz_set_ui(seven, 7);
    mpz_set_ui(eight, 8);

    // Parse exponent from command line arguments.
    if (argc != 2 || mpz_set_str(exponent, argv[1], 10) != 0)
    {
        fprintf(stderr, "Error: invalid exponent\n");
        exit(EXIT_FAILURE);
    }

    // Check if exponent is prime.
    if (mpz_probab_prime_p(exponent, 20) != 2)
    {
        printf("Exponent is not Prime !\n");
        exit(0);
    }

    mpz_mul(exponent2, exponent, two);
    // Calculate bigNumber = 2^exponent - 1.
    mpz_pow_ui(bigNumber, two, atoi(argv[1]));
    mpz_sub(bigNumber, bigNumber, one);
    gmp_printf("Number  : %Zd\n", bigNumber);

    // Calculate initial starting value.
    mpz_sqrt(start, bigNumber);
    gmp_printf("Sqrt    : %Zd\n", start);

    // Decrement start until it is 1 modulo p.
    while (mpz_cmp(mod, one) != 0 || !mpz_odd_p(start))
    {
        mpz_sub(start, start, one);
        mpz_mod(mod, start, exponent);
    }
    gmp_printf("Start   : %Zd\n", start);

    // Main loop: decrement start by p * 2 and check for factors.
    while (1)
    {
        mpz_mod(mod, start, eight);
        if (mpz_cmp(mod, one) == 0 || mpz_cmp(mod, seven) == 0)
        {
            if (mpz_cmp(start, one) < 0)
            {
                printf("Mersenne Prime !\n");
                exit(0);
            }
            switch (mpz_probab_prime_p(start, 50))
            {
            case 1:
            case 2:
                // gmp_printf("Check :  %Zd\n", start);
                mpz_mod(mod, bigNumber, start);
                if (mpz_cmp(mod, zero) == 0)
                {
                    gmp_printf("Factor  : %Zd\n", start);
                    exit(0);
                }
                break;
            case 0:
            default:
                break;
            }
        }
        mpz_sub(start, start, exponent2);
    }
}
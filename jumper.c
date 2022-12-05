#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <signal.h>
#include <time.h>

mpz_t zero, one, two, seven, eight, base, base2, bigNumber, start, check, mod;
int ctrl_c_count = 0;
time_t last_ctrl_c_time = 0;

static void sig_handler(int _)
{
    ctrl_c_count++;

    gmp_printf("\nCurrent : %Zd\n", start);
    FILE *fptr;
    char buffer[32]; // The filename buffer.
    snprintf(buffer, sizeof(char) * 32, "%s.res", mpz_get_str(buffer, 10, base));
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

    mpz_inits(zero, one, two, seven, eight, base, base2, bigNumber, start, check, mod, (mpz_ptr)0);
    mpz_set_ui(one, 1);
    mpz_set_ui(two, 2);
    mpz_set_ui(seven, 7);
    mpz_set_ui(eight, 8);
    mpz_set_str(base, argv[1], 10);
    mpz_mul(base2, base, two);

    if (mpz_probab_prime_p(base, 20) == 2)
    {
        mpz_pow_ui(bigNumber, two, atoi(argv[1]));
        mpz_sub(bigNumber, bigNumber, one);
        gmp_printf("Number  : %Zd\n", bigNumber);

        mpz_sqrt(start, bigNumber);
        gmp_printf("Sqrt    : %Zd\n", start);
        // exit(0);

        while (1)
        {
            mpz_mod(mod, start, base);
            if (mpz_cmp(mod, one) == 0)
            {
                if (mpz_odd_p(start))
                    break;
            }
            mpz_sub(start, start, one);
        }
        gmp_printf("Start   : %Zd\n", start);
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
                        mpz_out_str(stdout, 10, start);
                        gmp_printf("\n%Zd\n", mod);
                        exit(0);
                    }
                    break;
                case 0:
                default:
                    break;
                }
            }
            mpz_sub(start, start, base2);
        }
    }
    else
    {
        printf("Base is not Prime !\n");
        exit(0);
    }
}
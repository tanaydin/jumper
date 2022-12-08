#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <signal.h>
#include <time.h>

// Initialize variables.
mpz_t zero, one, two, seven, eight, exponent, exponent2, exponent8, bigNumber, start, checkMod1, checkMod7, mod;
int ctrl_c_count = 0;
time_t last_ctrl_c_time = 0;

static void sig_handler(int _)
{
    ctrl_c_count++;
    gmp_printf("\nCheckMod1  : %Zd\n", checkMod1);
    gmp_printf("CheckMod7  : %Zd\n", checkMod7);
    // Gonna use this for resuming, commenting out for now.
    // FILE *fptr;
    // char buffer[32]; // The filename buffer.
    // snprintf(buffer, sizeof(char) * 32, "%s.res", mpz_get_str(buffer, 10, exponent));
    // fptr = fopen(buffer, "w");
    // gmp_fprintf(fptr, "%Zd", start);
    // fclose(fptr);
    if (ctrl_c_count >= 2 && time(NULL) - last_ctrl_c_time <= 2)
    {
        printf("Exiting program...\n");
        exit(0);
    }
    else
    {
        last_ctrl_c_time = time(NULL);
    }
}

void check_factor(mpz_t number_to_check, mpz_t factor_to_check)
{
    // Checking if number is probably prime
    switch (mpz_probab_prime_p(factor_to_check, 50))
    {
    case 1:
    case 2:
        mpz_mod(mod, number_to_check, factor_to_check);
        if (mpz_cmp(mod, zero) == 0)
        {
            gmp_printf("\nFactor  : %Zd\n", factor_to_check);
            exit(0);
        }
        break;
    case 0:
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sig_handler);

    mpz_inits(zero, one, two, seven, eight, exponent, exponent2, exponent8, bigNumber, start, checkMod1, checkMod7, mod, (mpz_ptr)0);
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

    // Calculate exponent * 2 for using it while searching for a good number to start (1 in mod exponent).
    mpz_mul(exponent2, exponent, two);

    // Calculate exponent * 8 for keeping factoring numbers 1 or 7 in mod 8
    mpz_mul(exponent8, exponent, eight);

    // Calculate bigNumber = 2^exponent - 1.
    mpz_pow_ui(bigNumber, two, atoi(argv[1]));
    mpz_sub(bigNumber, bigNumber, one);
    gmp_printf("Number     : %Zd\n", bigNumber);

    // Calculate initial starting value. square root of the big number
    mpz_sqrt(start, bigNumber);
    // gmp_printf("Sqrt    : %Zd\n", start);

    // Decrement start until it is 1 modulo p.
    while (mpz_cmp(mod, one) != 0 || !mpz_odd_p(start))
    {
        mpz_sub(start, start, one);
        mpz_mod(mod, start, exponent);
    }
    // gmp_printf("Start   : %Zd\n", start);

    // Calculating two starting points 1 or 7 in mod 8.
    while (mpz_cmp(checkMod1, zero) == 0 || mpz_cmp(checkMod7, zero) == 0)
    {
        mpz_mod(mod, start, eight);
        if (mpz_cmp(checkMod1, zero) == 0 && mpz_cmp(mod, one) == 0)
        {
            mpz_set(checkMod1, start);
        }
        if (mpz_cmp(checkMod7, zero) == 0 && mpz_cmp(mod, seven) == 0)
        {
            mpz_set(checkMod7, start);
        }
        mpz_sub(start, start, exponent2);
    }
    // gmp_printf("checkMod1   : %Zd\n", checkMod1);
    // gmp_printf("checkMod7   : %Zd\n", checkMod7);

    // Main loop to check numbers
    while (mpz_cmp(checkMod1, zero) > 0 && mpz_cmp(checkMod7, zero) > 0)
    {
        check_factor(bigNumber, checkMod1);
        mpz_sub(checkMod1, checkMod1, exponent8);

        check_factor(bigNumber, checkMod7);
        mpz_sub(checkMod7, checkMod7, exponent8);
    }
    printf("\nMersenne Prime !\n");
    exit(0);
}

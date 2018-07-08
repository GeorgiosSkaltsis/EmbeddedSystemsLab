#include <stdlib.h>
#include <check.h>

// Define the suite builders here
Suite * add_pc_terminal_suite(void);
Suite * add_fixedpoint_suite(void);
Suite * add_flightmodes_suite(void);
Suite * add_transitions_suite(void);

int run(Suite* (*constructor) (void));
int main(int argc, char **argv);

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#define GET_USER_WEIGHT 460
#define SET_USER_WEIGHT 461

int test_get_case(int uid);
int test_set_case(int uid, int weight);
int get_tests();
int set_tests();

int expected_weights[65536] = {[0 ... 65535] = 10};

int main(){
    printf("\nErrno of type EACCES: %d and EINVAL: %d\n\n", EACCES, EINVAL);

    sleep(2);

    int get_tests_signal = get_tests();

    int set_tests_signal = set_tests();

    if (set_tests_signal == -1 || get_tests_signal == -1)
        return -1;

    printf("All tests passed!\n\n");

    for(int i = 0 ; i < 65536 ; i++) // reset user weights
        syscall(SET_USER_WEIGHT, i, 10);

    return 0;
}

int get_tests(){
    
    printf("Initiating get syscall test cases:\n\n");
    sleep(1);

    printf("Initiating uid < -1 test cases:\n\n");  /* uid < -1*/
    sleep(0.5);

    for(int i = /*INT_MIN*/ -2 ; i < -1 ; i++){
        int test_result = test_get_case(i);
        if(test_result != -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
    }

    printf("Initiating uid = -1 test case:");  /* uid = -1*/
    sleep(0.5);

    {
        int uid = getuid();
        if (uid < 0 || uid > 65535){
            printf("Current user uid is out of range.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        int test_result = test_get_case(-1);
        if(test_result == -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        if(test_result != expected_weights[uid]){
            printf("expected_weights[%d] = %d and test_result = %d\n", uid, expected_weights[uid], test_result);
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
    }

    printf("Initiating uid > -1 and uid < 65536 test cases:\n\n"); /* uid > -1 and uid < 65535*/
    sleep(0.5);

    for(int i = 0 ; i < 65536 ; i++) {
        int test_result = test_get_case(i);
        if(test_result == -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        if(test_result != expected_weights[i]){
            printf("expected_weights[%d] = %d and test_result = %d\n", i, expected_weights[i], test_result);
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
    }

    printf("Initiating uid > 65535 test cases:\n\n"); /* uid > 65535*/

    for(int i = 65536; i <= 65536 /*INT_MAX*/ ; i++) {
        int test_result = test_get_case(i);
        if(test_result != -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
    }

    printf("All get tests passed!\n\n");

    return 0;
}

int set_tests(){

    printf("Initiating /* weight < 0*/ test cases:\n\n");  /* weight < 0*/
    sleep(1);

    for(int i = /*INT_MIN*/ -2 ; i < -1 ; i++){
        int test_result = test_set_case(i, -1);
        if(test_result != -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
    }

    printf("Initiating uid < -1 and weight > 0 test cases:\n\n");  /* uid < -1, weight > 0*/
    sleep(0.5);

    for(int i = /*INT_MIN*/ -2 ; i < -1 ; i++){
        int test_result = test_set_case(i, 1);
        if(test_result != -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
    }

    printf("Initiating uid = -1 and weight  > 0 test cases:\n\n"); /* uid = -1, weight > 0*/
    sleep(0.5);

    {
        int uid = getuid();
        if (uid < 0 || uid > 65535){
            printf("Current user uid is out of range.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        int weight_before_set = expected_weights[uid];
        int test_result = test_set_case(-1, expected_weights[uid] + 1);
        if(test_result == -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        if(test_result != weight_before_set + 1){
            printf("weight_before_set = %d and test_result = %d\n", weight_before_set, test_result);
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        expected_weights[uid] = test_result;
    }

    printf("Initiating uid > -1 and uid < 65536, weight  > 0 test cases:\n\n"); /* uid = -1 and uid < 65536, weight > 0*/
    sleep(0.5);

    for(int i = 0 ; i < 65536 ; i++) {
        int weight_before_set = expected_weights[i];
        int test_result = test_set_case(i, expected_weights[i] + 1);
        if(test_result == -1){
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        if(test_result != weight_before_set + 1){
            printf("weight_before_set = %d and test_result = %d\n", weight_before_set, test_result);
            printf("Unexpected test result.\nENDING TEST PROGRAM WITH FAILURE.\n\n");
            return -1;
        }
        expected_weights[i] = test_result;
    }

    printf("Initiating  uid > 65535 and weight > 0* test cases:\n\n"); /* uid > 65535, weight > 0*/
    sleep(0.5);

    for(int i = 65536; i <= 65536 /*INT_MAX*/ ; i++) {
        int test_result = test_set_case(i, 1);
        if(test_result != -1)
            return -1;
    }  

    printf("All set tests passed!\n\n");

    return 0;
}

int test_get_case(int uid){
    printf("\nInitiating get test case uid = %d:\n", uid);

    int weight = syscall(GET_USER_WEIGHT, uid);

    if (weight == -1){
        printf("Error: Get test case uid = %d finished with error: %d.\n\n", uid, errno);
        return -1;
    }
    
    printf("User %d has weight = %d.\n", uid, weight);

    printf("Get test case uid = %d ended successfully\n\n", uid);

    return weight;
}

int test_set_case(int uid, int weight){
    printf("\nInitiating set test case uid = %d and weight = %d:\n", uid, weight);

    int weight_before_set = syscall(GET_USER_WEIGHT, uid);

    if (weight_before_set == -1){
        printf("Error: %d.\n\n", errno);
        return -1;
    }

    int set_weight = syscall(SET_USER_WEIGHT, uid, weight);

    if (set_weight == -1){
        printf("Error: Set test case uid = %d and weight = %d finished with error: %d.\n\n", uid, weight, errno);
        return -1;
    }

    int weight_after_set = syscall(GET_USER_WEIGHT, uid);

    if (weight_after_set == -1){
        printf("Error: Set test case uid = %d and weight = %d finished with error: %d.\n\n", uid, weight, errno);
        return -1;
    }

    printf("User %d had weight = %d before set operation.\n", uid, weight_before_set);

    printf("User %d has weight = %d after set operation.\n", uid, weight_after_set);

    printf("Get test case uid = %d and weight = %d ended successfully\n\n", uid, weight);

    return weight_after_set;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../dbms/db_kernel.h"
#include "../dbms/file_manager/data_file.h"
#include "../dbms/db_export_json.h"


struct DataFile * warehouse;
struct DataFile * district;
struct DataFile * customer;
struct DataFile * history;
struct DataFile * new_order;
struct DataFile * order;
struct DataFile * order_line;
struct DataFile * stock;

struct DataFile * map_file_id(int file_id){
       if(file_id == 11) return warehouse;
       if(file_id == 22) return district;
       if(file_id == 33) return customer;
       if(file_id == 44) return history;
       if(file_id == 55) return new_order;
       if(file_id == 66) return order;
       if(file_id == 77) return order_line;
       if(file_id == 88) return stock;
       return NULL;
}

 
int main(void) { //gcc src/tests/test_buffer_requests.c -o database -Wall -Wextra

	start_database();
    
    //------------------ Initializing data files -----------------
    warehouse  = data_file_open("warehouse",  CATALOG_DATA_FOLDER);
    district   = data_file_open("district",   CATALOG_DATA_FOLDER);
    customer   = data_file_open("customer",   CATALOG_DATA_FOLDER);
    history    = data_file_open("history",    CATALOG_DATA_FOLDER);
    new_order  = data_file_open("new_order",  CATALOG_DATA_FOLDER);
    order      = data_file_open("order",      CATALOG_DATA_FOLDER);
    order_line = data_file_open("order_line", CATALOG_DATA_FOLDER);
    stock      = data_file_open("stock",      CATALOG_DATA_FOLDER);
    //-------------------------------------------------------------
    printf("\n---------------------------------------------------------------\n");
    
    struct timeval start_time, end_time;
    char operation;
	int data_file_id = 11;
    int block_id;
       
    FILE * file = fopen("C:\\Users\\Gustavo\\Desktop\\workloads\\workloadGerado---zipf-15m-a1-n1m-80w-no-burst.txt", "r");
    if (file == NULL) {
		printf("\n workload not found \n");
        return -1;
	}    

 
	gettimeofday(&start_time, NULL); // Start Time
    while (fscanf(file, "%c;%d\n", &operation, &block_id) > 0) {
       
       
        //printf("\nRequest ---> %c[%d-%d]", operation, map_file_id(data_file_id)->file_id, block_id);		
        
        if(operation == 'R')buffer_request_page( map_file_id(data_file_id)->file_id, block_id, READ_REQUEST);
        if(operation == 'W')buffer_request_page( map_file_id(data_file_id)->file_id, block_id, WRITE_REQUEST);
        //buffer_print_policy();
    }  

    gettimeofday(&end_time, NULL); // End Time

    //buffer_flush(); 
	buffer_print_statistics();
    // Show Execution Time in Seconds
    double time =  ( ((double) ((double)end_time.tv_usec - (double)start_time.tv_usec)) / 1000000 ) +  ( (double) ((double)end_time.tv_sec - (double)start_time.tv_sec) )  ;
    
    printf("\nExecution Time = %f seconds\n", time);

    
    
    export_json_final_result(BUFFER_POLICY_NAME, BUFFER_SIZE, flush_operations, hit_operations, time);
	//printf("\nPress Any Key to Exit\n");
	//getchar();
	return 0;
}
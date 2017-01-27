//C Standard Libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

//CUnit Framework
#include <CUnit/Basic.h>

//Project Includes
#include "../src/ringbuf.h"

struct ringbuffer test_buffer = {{0}, 0, 0, true};

int init_suite1(void) {
	//Malloc Here
	return 0;
}

int clean_suite1(void) {
	//Free Here
	return 0;
}

void clean_buffer(struct ringbuffer* buffer) {
	for(int i=0;i<BUFFER_SIZE;i++) {
		(buffer->buffer)[i] = 0;
	}
	buffer->head = 0;
	buffer->tail = 0;
	buffer->empty = true;
}
	

void TEST_buffer_read(void) {
	clean_buffer(&test_buffer);
	
	for(int i=0;i<BUFFER_SIZE;i++) {
		test_buffer.buffer[i] = i;
	}
	test_buffer.empty = false;
	
	for(int i=0;i<BUFFER_SIZE;i++) {
		char rvalue = buffer_read(&test_buffer);
		CU_ASSERT(i == rvalue);
	}
	
	for(int i=0;i<BUFFER_SIZE;i++) {
		test_buffer.buffer[i] = i;
	}
	test_buffer.empty = false;
	
	for(int i=0;i<BUFFER_SIZE;i++) {
		char rvalue = buffer_read(&test_buffer);
		CU_ASSERT(i == rvalue);
	}
}

void TEST_buffer_write(void) {
	clean_buffer(&test_buffer);
	
	for(int i=0;i<BUFFER_SIZE;i++) {
		buffer_write(&test_buffer, i);
	}
	
	buffer_write(&test_buffer, '@');
	buffer_write(&test_buffer, '$');
	
	for(int i=2;i<BUFFER_SIZE;i++) {
		CU_ASSERT(i == buffer_read(&test_buffer));
	}
	CU_ASSERT('@' == buffer_read(&test_buffer));
	CU_ASSERT('$' == buffer_read(&test_buffer));
	
	for(int i=0;i<BUFFER_SIZE;i++) {
		buffer_write(&test_buffer, i);
	}
	
	buffer_write(&test_buffer, '@');
	buffer_write(&test_buffer, '$');
	
	for(int i=2;i<BUFFER_SIZE;i++) {
		CU_ASSERT(i == buffer_read(&test_buffer));
	}
	CU_ASSERT('@' == buffer_read(&test_buffer));
	CU_ASSERT('$' == buffer_read(&test_buffer));

}

void TEST_buffer_read_str(void) {
	clean_buffer(&test_buffer);
	
	char ref_str [] = "Hello World";
	int len = strlen(ref_str);
	strcpy((*uint8_t)&(test_buffer.buffer[0]), &ref_str);
	test_buffer.head = test_buffer.tail + len;
	
	char rvalue[len]; 
	buffer_read_str(&test_buffer, &rvalue);
	CU_ASSERT(strcmp(rvalue ,ref_str));
}

void TEST_buffer_write_str(void) {
	clean_buffer(&test_buffer);
	
	
}

void TEST_buffer_full(void) {
	clean_buffer(&test_buffer);
	
	CU_ASSERT_FALSE(buffer_full(&test_buffer));
	
	for(int i=0;i<BUFFER_SIZE;i++) {
		buffer_write(&test_buffer, i);
	}
	
	CU_ASSERT(buffer_full(&test_buffer));
}

void TEST_buffer_empty(void) {
	clean_buffer(&test_buffer);
	
	CU_ASSERT(buffer_empty(&test_buffer));
	
	for(int i=0;i<5;i++) 
		buffer_write(&test_buffer, 1);
		
	
	CU_ASSERT_FALSE(buffer_empty(&test_buffer));
	
	for(int i=0;i<5;i++) 
		buffer_read(&test_buffer);
	
	CU_ASSERT(buffer_empty(&test_buffer));
	
	for(int i=0;i<5;i++) 
		buffer_write(&test_buffer, 1);
		
	
	CU_ASSERT_FALSE(buffer_empty(&test_buffer));
	
	for(int i=0;i<5;i++) 
		buffer_read(&test_buffer);
	
	CU_ASSERT(buffer_empty(&test_buffer));
}

int main(void) {
	CU_pSuite pSuite = NULL;
	
	CU_initialize_registry();
	
	pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
	
	CU_add_test(pSuite, "Test of buffer_read", TEST_buffer_read);
	CU_add_test(pSuite, "Test of buffer_write", TEST_buffer_write);
	CU_add_test(pSuite, "Test of buffer_read_str", TEST_buffer_read_str);
	CU_add_test(pSuite, "Test of buffer_write_str", TEST_buffer_write_str);
	CU_add_test(pSuite, "Test of buffer_full", TEST_buffer_full);
	CU_add_test(pSuite, "Test of buffer_empty", TEST_buffer_empty);
	
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

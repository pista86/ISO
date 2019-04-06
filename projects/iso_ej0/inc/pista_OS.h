


typedef void *(*task_type)(void *);


void pista_OS_init(void);
void init_stack(uint32_t stack[], uint32_t stack_size_bytes, uint32_t * sp,
		task_type entry_point, void * arg);

template<typename T, int count>
class mem_pool {
    struct pool_node {
        T data;
        pool_node *next;
    };
    pool_node head;
    pool_node *mem, *mem_begin, *mem_end;
    
public:
    mem_pool() {
        mem = (pool_node*)malloc(sizeof(pool_node)*count);
        mem_begin = mem;
        mem_end = mem + count;
        head.next = NULL;
    }
    virtual ~mem_pool() {
        free(mem);
    }
    void *allocate() {
        if(head.next == NULL) {
            void *ans = mem_begin;
            ++mem_begin;
            return ans;
        } else {
            void *ans = head.next;
            head.next = head.next->next;
            return ans;
        }
    }
    void deallocate(void *ptr) {
        if(ptr == NULL) return;
        pool_node *new_node = (pool_node*)ptr;
        new_node->next = head.next;
        head.next = new_node;
    }
};
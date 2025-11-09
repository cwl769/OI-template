#include <cstdint>
#include <cstddef>

#ifndef DS_RBTREE_COLOR
#define DS_RBTREE_COLOR

#define DS_RBTREE_RED 0
#define DS_RBTREE_BLACK 1

#endif

template<typename T>
struct ds_rbtree_node {
    uint8_t color;
    ds_rbtree_node *father, *son[2];
    T data;
};

template<typename T>
struct ds_rbtree {
    ds_rbtree_node<T> vir_root;
    void (*modify)(ds_rbtree_node<T>*);
};

static void * cur_tree;

template<typename T>
static uint8_t which_son(ds_rbtree_node<T> * node) {
    return (node->father->son[0] == node) ? 0 : 1 ;
}

template<typename T>
static inline uint8_t getcol(ds_rbtree_node<T> * node) {
    return (node == NULL) ? DS_RBTREE_BLACK : node->color ;
}

template<typename T>
static void rotate(ds_rbtree_node<T> * node) {
    uint8_t which = which_son(node);
    ds_rbtree_node<T> * fa = node->father;
    ds_rbtree_node<T> * gfa = fa->father;
    ds_rbtree_node<T> * s = node->son[which^1];

    gfa->son[which_son(fa)] = node;
    node->son[which^1] = fa;
    fa->son[which] = s;
    if(s != NULL) s->father = fa;
    fa->father = node;
    node->father = gfa;

    ((ds_rbtree<T>*)cur_tree)->modify(fa);
    ((ds_rbtree<T>*)cur_tree)->modify(node);/*the order of modify should not be changed*/
}

template<typename T>
static ds_rbtree_node<T>* prev(ds_rbtree_node<T> * node) {
    ds_rbtree_node<T> *tar = node->son[0];
    while(tar != NULL) {
        if(tar->son[1] == NULL)
            return tar;
        else
            tar = tar->son[1];
    }
    return tar;
}

template<typename T>
static ds_rbtree_node<T>* next(ds_rbtree_node<T> * node) {
    ds_rbtree_node<T> *tar = node->son[1];
    while(tar != NULL) {
        if(tar->son[0] == NULL)
            return tar;
        else
            tar = tar->son[0];
    }
    return tar;
}

template<typename T>
static void insert_fixup(ds_rbtree_node<T> * node) {
    if(node->father == &((ds_rbtree<T>*)cur_tree)->vir_root) {
        node->color = DS_RBTREE_BLACK;
        return;
    }
    while(node->father->color == DS_RBTREE_RED) {
        ds_rbtree_node<T> * fa = node->father;
        ds_rbtree_node<T> * gfa = fa->father;
        ds_rbtree_node<T> * unc = gfa->son[which_son(fa)^1];
        if(getcol(unc) == DS_RBTREE_RED) {
            fa->color = DS_RBTREE_BLACK;
            unc->color = DS_RBTREE_BLACK;
            gfa->color = DS_RBTREE_RED;
            node = gfa;
            if(node->father == &((ds_rbtree<T>*)cur_tree)->vir_root) {
                node->color = DS_RBTREE_BLACK;
                return;
            }
            continue;
        }
        if(which_son(fa) != which_son(node)) {
            rotate(node);
            node = fa;
            fa = node->father;
        }
        rotate(fa);
        fa->color = DS_RBTREE_BLACK;
        gfa->color = DS_RBTREE_RED;
        break;
    }
}

template<typename T>
static void erase_fixup(ds_rbtree_node<T> * node) {
    while(node->color == DS_RBTREE_BLACK && node->father != &((ds_rbtree<T>*)cur_tree)->vir_root) {
        ds_rbtree_node<T> *fa = node->father;
        ds_rbtree_node<T> *sil = fa->son[which_son(node)^1];
        if(getcol(sil) == DS_RBTREE_RED) {
            rotate(sil);
            sil->color = DS_RBTREE_BLACK;
            fa->color = DS_RBTREE_RED;
            sil = fa->son[which_son(node)^1];
        }
        if(getcol(sil->son[0]) == DS_RBTREE_BLACK && getcol(sil->son[1]) == DS_RBTREE_BLACK) {
            sil->color = DS_RBTREE_RED;
            if(getcol(fa)==DS_RBTREE_BLACK) {
                node = fa;
                continue;
            } else {
                fa->color = DS_RBTREE_BLACK;
                break;
            }
        }
        if(getcol(sil->son[which_son(sil)]) == DS_RBTREE_BLACK && getcol(sil->son[which_son(sil)^1]) == DS_RBTREE_RED) {
            rotate(sil->son[which_son(sil)^1]);
            sil->father->color = DS_RBTREE_BLACK;
            sil->color = DS_RBTREE_RED;
            sil = sil->father;
        }
        sil->color = fa->color;
        fa->color = sil->son[which_son(sil)]->color = DS_RBTREE_BLACK;
        rotate(sil);
        break;
    }
}

template<typename T>
static void ds_rbtree_free_node(ds_rbtree_node<T>*node) {
    if(node == NULL) return;
    ds_rbtree_free_node(node->son[0]);
    ds_rbtree_free_node(node->son[1]);
    free(node);
}

template<typename T>
ds_rbtree_node<T>* ds_rbtree_insert(ds_rbtree<T> * tree, ds_rbtree_node<T> * node, uint8_t which, T data) {
    ((ds_rbtree<T>*)cur_tree) = tree;
    ds_rbtree_node<T> * new_node = (ds_rbtree_node<T>*)malloc(sizeof(ds_rbtree_node<T>));
    new_node->color = DS_RBTREE_RED;
    new_node->father = node;
    new_node->son[0] = new_node->son[1] = NULL;
    new_node->data = data;
    node->son[which] = new_node;
    insert_fixup(new_node);
    for(ds_rbtree_node<T>*p=new_node->father;p!=&((ds_rbtree<T>*)cur_tree)->vir_root;p=p->father) {
        ((ds_rbtree<T>*)cur_tree)->modify(p);
    }
    return new_node;
}

template<typename T>
void ds_rbtree_erase(ds_rbtree * tree, ds_rbtree_node<T> * node) {
    ((ds_rbtree<T>*)cur_tree) = tree;
    if(node->son[0] != NULL && node->son[1] != NULL) {
        ds_rbtree_node<T> *s = prev(node);
        void * tmp = s->data;
        s->data = node->data;
        node->data = tmp;
        node = s;
    }
    if(node->son[0] == NULL && node->son[1] == NULL && node->father != &((ds_rbtree<T>*)cur_tree)->vir_root && node->color == DS_RBTREE_BLACK)
        erase_fixup(node);
    ds_rbtree_node<T> * rep = NULL;
    if(node->son[0] != NULL) {
        rep = node->son[0];
    }
    if(node->son[1] != NULL) {
        rep = node->son[1];
    }
    if(rep != NULL) {
        rep->father = node->father;
        rep->color = DS_RBTREE_BLACK;
    }
    node->father->son[which_son(node)] = rep;

    for(ds_rbtree_node<T>*p=node->father;p!=&((ds_rbtree<T>*)cur_tree)->vir_root;p=p->father) {
        ((ds_rbtree<T>*)cur_tree)->modify(p);
    }
    free(node);
}

template<typename T>
ds_rbtree_node<T>* ds_rbtree_prev(ds_rbtree_node<T>*node) {
    return prev(node);
}

template<typename T>
ds_rbtree_node<T>* ds_rbtree_next(ds_rbtree_node<T>*node) {
    return next(node);
}

template<typename T>
ds_rbtree_node<T>* ds_rbtree_getroot(ds_rbtree*tree) {
    return tree->vir_root.son[0];
}

template<typename T>
void ds_rbtree_create(T p){
    ds_rbtree *tree = (ds_rbtree*)p;
    tree->modify = ds_rbtree_modify_default;
    tree->vir_root.color = DS_RBTREE_BLACK;
    tree->vir_root.father = NULL;
    tree->vir_root.son[0] = tree->vir_root.son[1] = NULL;
    tree->vir_root.data = NULL;
}

template<typename T>
void ds_rbtree_modify_default(ds_rbtree_node<T> *)
{
    // do nothing
}

void ds_rbtree_free(ds_rbtree*tree) {
    ds_rbtree_free_node(&tree->vir_root);
}
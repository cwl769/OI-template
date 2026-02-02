#include <cstdio>
#include <cstdlib>
#include <cstring>


/**
 * @note TODO should be implemented by yourself!
 */
class Segtree final {
private:
    struct Node final{
        /*TODO*/
        //data
        
        /*TODO end*/
        Node *son[2];
        /*TODO*/
        //constructor
        Node(){}
        /*TODO end*/
        void pushup() {
            /*TODO*/
            //how to modify data from son
            
            /*TODO end*/
        }
        static void update(Node *p/*TODO arg list*/) {
            /*TODO*/
            //how to modify sons' data from tags
            
            /*TODO end*/
        }
        void pushdown(int l, int r) {
            if(son[0] == NULL)
                son[0] = new Node;
            if(son[1] == NULL)
                son[1] = new Node;
            int mid = ((l+r)>>1);
            /*TODO*/
            //update sons using the function
            update(son[0]/*TODO arg list*/);
            update(son[1]/*TODO arg list*/);
            //clear current tag
            
            /*TODO end*/
        }

        void change(int l, int r, int ql, int qr/*TODO arg list*/) {
            if(ql<=l&&r<=qr) {
                update(this/*TODO arg list*/);
                return;
            }
            int mid = ((l+r)>>1);
            pushdown(l, r);
            if(ql<=mid) {
                son[0]->change(l, mid, ql, qr/*TODO arg list*/);
            }
            if(qr>mid) {
                son[1]->change(mid+1, r, ql, qr/*TODO arg list*/);
            }
            pushup();
        }

        /*TODO return type*/void query(int l, int r, int ql, int qr) {
            if(ql<=l&&r<=qr) {
                /*TODO*/
                //return the answer

                /*TODO end*/
            }
            int mid = ((l+r)>>1);
            pushdown(l, r);
            /*TODO*/
            //how to calculate answer

            /*TODO end*/
            pushup();

            /*TODO*/
            //return the answer

            /*TODO end*/
        }
        ~Node() {
            delete son[0];
            delete son[1];
        }
    };
    Node *root;
    int L, R;
public:
    Segtree(int l, int r) {
        root = new Node;
        L = l;
        R = r;
    }
    /*TODO*/
    //define some function here to simplify operations

    /*TODO end*/
    ~Segtree() {
        delete root;
    }
};
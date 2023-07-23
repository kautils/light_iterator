
#include "light_iterator.h"
#include <stdint.h>
#include <bits\move.h>
#include <exception>

struct non_continuous_typecheck_exception : std::exception{
    const char*what() const noexcept{
        static const char * kMsg = "non_continuous_iterator : different type is detected. container_type::iterator and decltype(VALUE_YOU_SPECIFIED) must be the same.";
        return kMsg;
    }
};
struct continuous_typecheck_exception : std::exception{
    const char*what() const noexcept{
        static const char * kMsg = "continuous_iterator : different type is detected. container_type::iterator and decltype(VALUE_YOU_SPECIFIED) must be the same.";
        return kMsg;
    }
};



#define __i(v) reinterpret_cast<uintptr_t>(v)
#define __v(v) reinterpret_cast<void*>(v)

struct continuous_internal{
    void * end = 0;
    void * beg = 0;
    void * cur=0;
    int step=1;
    bool fst = true;
};

Continuous::Continuous() : m(new continuous_internal){}
Continuous::~Continuous(){ delete m; }
Continuous Continuous::factory(void * beg, void * end,int step,int size0,int size1){
    auto res= Continuous();
    res.set(beg,end,step,size0,size1);
    return res;
}

void Continuous::set(void * beg, void * end,int step,int size0,int size1) const{
    if(size0!=size1) throw continuous_typecheck_exception{};
    m->beg = beg;
    m->end = end;
    m->cur = beg;
    m->step=step;
}
bool Continuous::next() const{
    if(m->fst)m->fst=false;
    else m->cur= __v(__i(m->cur) + m->step);
    return m->cur < m->end;
}
void Continuous::reset()const { m->fst = true;m->cur = m->beg; }
void * Continuous::current() const{ return m->cur; }
    

struct non_continuous_map_internal{
    void * obj = 0;
    void * beg = 0;
    void * end = 0;
    void * cur=0;
    void * (*new_begin)();
    void * (*new_end)();
    void  (*del)(void *);
    void (*step)(void * element_as_voidptr);
    bool (*is_end)(void * cur,void * end);
    bool fst = true;
};

NonContinuous::~NonContinuous(){ 
    if(m){ m->del(m->beg);m->del(m->end);delete m; }
}


bool NonContinuous::next() const {
    if(m->fst) m->fst=false;
    else m->step(m->cur);
    return m->is_end(m->cur,m->end);
}


NonContinuous::NonContinuous(NonContinuous && r) : m(r.m){ r.m=nullptr; }
NonContinuous NonContinuous::factory(void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * )
         ,void (*step)(void * element_as_voidptr)
         ,bool (*is_end)(void * cur,void * end)
         ,int size0,int size1
        ){
    auto res=NonContinuous{};
    res.set(obj,new_begin,new_end,del,step,is_end,size0,size1);
    return std::move(res);
}

void NonContinuous::set(void * obj
         ,void * (*new_begin)(void * obj)
         ,void * (*new_end)(void * obj)
         ,void (*del)(void * )
         ,void (*step)(void * element_as_voidptr)
         ,bool (*is_end)(void * cur,void * end)
        ,int size0,int size1
         ) {
    if(size0!=size1) throw non_continuous_typecheck_exception{};
    
    m->del= del;
    m->del(m->beg);
    m->del(m->end);
    m->beg= new_begin(obj);
    m->end= new_end(obj);
    m->step= step;
    m->is_end= is_end;
    m->cur= m->beg;
}

void NonContinuous::reset() const { m->fst=true;m->cur=m->beg; }
void* NonContinuous::current() const { return m->cur; }
NonContinuous::NonContinuous() : m(new non_continuous_map_internal){}



Continuous* continuous_itr_initialize(){ return new Continuous{}; }
void continuous_itr_setup(Continuous* m,void * beg, void * end,int step,int size0,int size1){ m->set(beg,end,step,size0,size1); }
void continuous_itr_finalize(Continuous* m){ delete m; }
bool continuous_itr_next(Continuous* m){ return m->next(); }
void continuous_itr_reset(Continuous* m){ m->reset(); }
void* continuous_itr_current(Continuous* m){ return m->current(); }


NonContinuous* non_continuous_itr_initialize(){ return new NonContinuous; }
void non_continuous_itr_finalize(NonContinuous* m){ delete m; }
void non_continuous_itr_setup(NonContinuous* m,void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1){ m->set(obj,new_begin,new_end,del,step,is_end,size0,size1); }
bool non_continuous_itr_next(NonContinuous* m){ return m->next(); }
void non_continuous_itr_reset(NonContinuous* m){ m->reset(); }
void* non_continuous_itr_current(NonContinuous* m){ return m->current(); }



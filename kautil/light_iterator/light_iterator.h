#ifndef KAUTIL_LIGHT_ITERATOR_LIGHT_ITERATOR_H
#define KAUTIL_LIGHT_ITERATOR_LIGHT_ITERATOR_H


struct Continuous; 
Continuous* continuous_itr_initialize();
void continuous_itr_setup(Continuous* m,void * beg, void * end,int step,int size0,int size1);
void continuous_itr_finalize(Continuous* m);
bool continuous_itr_next(Continuous* m);
void continuous_itr_reset(Continuous* m);
void* continuous_itr_current(Continuous* m);
#define continuous_itr_setup_util(m,type,arg) continuous_itr_setup(m,(arg).begin().base(),(arg).end().base(),sizeof(type::value_type),sizeof(type),sizeof( decltype(arg) ))
#define continuous_itr_setup_util_p(m,type,arg) continuous_itr_setup(m,(arg)->begin().base(),(arg)->end().base(),sizeof(type::value_type),sizeof(type),sizeof( decltype(*(arg)) ))


struct NonContinuous;
NonContinuous* non_continuous_itr_initialize();
void non_continuous_itr_finalize(NonContinuous* m);
void non_continuous_itr_setup(NonContinuous* m,void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1);
bool non_continuous_itr_next(NonContinuous* m);
void non_continuous_itr_reset(NonContinuous* m);
void* non_continuous_itr_current(NonContinuous* m);
#define non_continuous_itr_setup_util_p(itr,type,ptr) non_continuous_itr_setup(\
    itr\
    ,ptr\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->begin()); }\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->end()); }\
    ,[](void * itr){  delete reinterpret_cast<type::iterator*>(itr); }\
    ,[](void* v){ ++(*reinterpret_cast<type::iterator*>(v)); }\
    ,[](void* cur,void* end){ return *reinterpret_cast<type::iterator*>(cur) != *reinterpret_cast<type::iterator*>(end); },sizeof(type),sizeof(decltype(*(ptr))));

#define non_continuous_itr_setup_util_declared_p(itr,prefix,ptr) non_continuous_itr_setup(itr, (ptr) ,prefix##_begin ,prefix##_end ,prefix##_delete ,prefix##_next ,prefix##_is_end,prefix##_object_size(),sizeof(decltype( *(ptr) )))

#define non_continuous_itr_setup_util(itr,type,obj) non_continuous_itr_setup_util_p(itr,type,&obj)
#define non_continuous_itr_setup_util_declared(itr,type,obj) non_continuous_itr_setup_util_declared_p(itr,type,&obj)




struct continuous_internal;
struct Continuous{
    friend Continuous* continuous_itr_initialize();
    ~Continuous();
    static Continuous factory(void * beg, void * end,int step,int size0,int size1);
    void set(void * beg, void * end,int step,int size0,int size1) const;
    bool next() const;
    void reset()const; 
    void * current() const;
private:
    Continuous();
    continuous_internal * m=0;
};

#define continuous_iterator(type,arg) Continuous::factory((arg).begin().base(),(arg).end().base(), sizeof(type::value_type),sizeof(type),sizeof( decltype(arg) ))
#define continuous_iterator_p(type,arg) Continuous::factory((arg)->begin().base(),(arg)->end().base(), sizeof(type::value_type),sizeof(type),sizeof( decltype(*(arg)) ))

struct non_continuous_map_internal;
struct NonContinuous{
    friend NonContinuous* non_continuous_itr_initialize();
    ~NonContinuous();
    NonContinuous(NonContinuous && r);
    bool next() const; 
    static NonContinuous factory(void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1);
    void set(void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1); 
    void reset() const ;
    void* current() const ;
    
private:
    NonContinuous(NonContinuous const& )=delete;
    NonContinuous& operator=(NonContinuous const&)=delete;
    NonContinuous() ;
    non_continuous_map_internal *m=0;
};




#define noncontinuous_iterator_p(type,ptr) NonContinuous::factory(\
    ptr\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->begin()); }\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->end()); }\
    ,[](void * itr){  delete reinterpret_cast<type::iterator*>(itr); }\
    ,[](void* v){ ++(*reinterpret_cast<type::iterator*>(v)); }\
    ,[](void* cur,void* end){ return *reinterpret_cast<type::iterator*>(cur) != *reinterpret_cast<type::iterator*>(end); },sizeof(type),sizeof(decltype(*(ptr))));

#define noncontinuous_iterator(type,obj) noncontinuous_iterator_p(type,&obj)


#define noncontinuous_generate_callback_functions(prefix,type)\
void * prefix##_begin(void * o){ return new type::iterator(reinterpret_cast<type*>(o)->begin()); }\
void * prefix##_end(void * o){ return new type::iterator(reinterpret_cast<type*>(o)->end()); }\
void prefix##_delete(void * itr){  delete reinterpret_cast<type::iterator*>(itr); }\
void prefix##_next(void* v){ ++(*reinterpret_cast<type::iterator*>(v)); }\
bool prefix##_is_end(void* cur,void* end){ return *reinterpret_cast<type::iterator*>(cur) != *reinterpret_cast<type::iterator*>(end); }\
int32_t prefix##_object_size(){ return sizeof(type); }

#define noncontinuous_iterator_declared_p(prefix,ptr) NonContinuous::factory( ptr ,prefix##_begin ,prefix##_end ,prefix##_delete ,prefix##_next ,prefix##_is_end,prefix##_object_size(),sizeof(decltype(*(ptr))))
#define noncontinuous_iterator_declared(prefix,obj) NonContinuous::factory( (&obj) ,prefix##_begin ,prefix##_end ,prefix##_delete ,prefix##_next ,prefix##_is_end,prefix##_object_size(),sizeof(decltype(obj)))





#endif
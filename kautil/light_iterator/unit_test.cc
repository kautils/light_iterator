
#ifdef TMAIN_LIGHT_ITERATOR

#include<stdio.h>
#include "light_iterator.h"
int tmain_light_iterator(int(*)(const char *,...));
int main(){ return tmain_light_iterator(printf); }


// below is exceptional. i wanted to do test with using conatiner .
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <string>


using map_s_s_t = std::map<std::string,std::string>;
kautil_noncontinuous_generate_callback_functions(map_s_s,map_s_s_t)

using umap_s_u64_t = std::unordered_map<std::string,uint64_t>;
kautil_noncontinuous_generate_callback_functions(map_s_u64,umap_s_u64_t)

#define export_vector_type std::vector<int> 
// below is expected. 
// this macro(export_vector_type std::vector<int>) is inside header of a module.
// in that module, vector is not included in header but included in source file.
// that module exports pointer to iterator.




int tmain_light_iterator(int(*__printf)(const char*,...)){

    
    using namespace kautil::light_iterator;
    
    
    for(;;){
        { // map
            using map_type=std::map<std::string,std::string>;
            using umap_type=std::unordered_map<std::string,std::string>;
            //auto m = umap_type{};{ // expected to throw exception  because of the unmatch of type
            auto m = map_type{};{
                m.insert({"test","some"});
                m.insert({"test2","some2"});
                m.insert({"test3","some3"});
            }

            {
                auto itr=kautil_noncontinuous_iterator_declared_p(map_s_s,&m);
                while(itr.next()){
                    printf("%s\n", (*reinterpret_cast<map_type::iterator*>(itr.current()))->second.data());
                    fflush(stdout);
                } 
            }

            {
                auto itr = kautil_non_continuous_itr_initialize();
                //kautil_non_continuous_itr_setup_util_p(itr,map_s_s_t,&m);
                kautil_non_continuous_itr_setup_util_declared_p(itr,map_s_s,&m);
                kautil_non_continuous_itr_setup_util_declared(itr,map_s_s,m);
                while(kautil_non_continuous_itr_next(itr)){
                    printf("%s\n", (*reinterpret_cast<map_type::iterator*>(kautil_non_continuous_itr_current(itr)))->second.data());
                }
                kautil_non_continuous_itr_finalize(itr);
            }
            
            
        }
        
        
        {// set
            using set_type = std::set<std::string>; 
            auto m2 = set_type{};{
                m2.insert("set0");
                m2.insert("set2");
                m2.insert("set3");
            }
            
            auto itr2=kautil_noncontinuous_iterator(set_type,m2);
            // auto itr2=noncontinuous_iterator_p(set_type,&m2);
            while(itr2.next()) printf("%s\n", (*reinterpret_cast<set_type::iterator*>(itr2.current()))->data());
        }


        {// vector
            using value_type = int;
            auto vec = std::vector<int>{1,2,3,4};

            {
                auto const& itr0 = kautil_continuous_iterator_p(export_vector_type,(&vec));
                while(itr0.next())__printf("++%d\n",*reinterpret_cast<value_type*>(itr0.current()));
                itr0.reset();
            }
            
            {
                auto itr = kautil_continuous_itr_initialize();
                kautil_continuous_itr_setup_util_p(itr,export_vector_type,&vec);
                //continuous_itr_setup_util(itr,export_vector_type,vec);
                while(kautil_continuous_itr_next(itr)) __printf("++%d\n",*reinterpret_cast<value_type*>(kautil_continuous_itr_current(itr)));
                kautil_continuous_itr_finalize(itr);
            }
        }
    
    }
    
    return 0;
}
#endif
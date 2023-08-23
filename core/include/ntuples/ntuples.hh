


#include <type_traits>


#include <iostream>
#include <vector>

#include <iomanip> 


namespace nt {


  template <class _Ty>
  using _Remove_cvref_t = std::remove_cv_t<std::remove_reference_t<_Ty>>;

  template<int N, typename... Ts> using NthTypeOf =
  typename std::tuple_element<N, std::tuple<Ts...>>::type;

  template <auto Start, auto End, auto Inc, class F>
  constexpr void constexpr_for(F&& f)
  {
    if constexpr (Start < End) {
      f(std::integral_constant<decltype(Start), Start>());
      constexpr_for < Start + Inc, End, Inc >(f);
    }
  }




  template <typename T1>
  struct type_container {
    using type = T1;
  };







  template <typename T1, typename T2>
  struct ax_type : T2 {
    T1 v = {};
    constexpr ax_type() {}
    constexpr ax_type(T1 t1) : v(std::move(t1)) {}
    constexpr ax_type operator=(T1 && t1) const  { return ax_type(std::move(t1)); }
    constexpr ax_type operator=(const T1 & t1) const  { return ax_type(std::move(t1)); }
    constexpr ax_type operator()(T1 && t1) const { return ax_type(std::move(t1)); }
    constexpr ax_type operator()(const T1 && t1) const { return ax_type(t1); }
    constexpr ax_type operator=(T1&& t1) { v = std::move(t1);  return *this; }
    constexpr ax_type operator=(const  T1& t1) { v = t1;  return *this; }
    using struct_maker = T2;
    using data_t = T1;

    operator T1() {
      return v;
    }



    friend std::ostream& operator<< (std::ostream& out, const ax_type& self) {
      out << self.get_name() << " : " << self.v;
      return out;
    }

private:
  T2::operator=;
  };





  template <typename T1, typename T2>
  struct ax_type2 : T1, T2 {

    constexpr ax_type2() = default;


    using struct_maker = T2;
    using data_t = T1;




  };

  template <typename T2, typename T3, typename T4>
  struct ax_name_container_base {
    using type = T2;
    using getter = T3;
    using name_getter = T4;
    constexpr ax_name_container_base() = default;
  };

    
  template <typename TBase>
  struct ax_name_container  : TBase{
    constexpr ax_name_container() = default;
    template <typename T>
    struct type_wrap {
      T val;
    };

    template <typename T>
    static constexpr  auto struct_maker123() {
      return  decltype(std::declval<type>() (std::declval<type_wrap<T> >())){};
    }

    template <typename T>
    static constexpr decltype(auto) get(T& t) {
      using   getter1 = decltype(std::declval<getter>()(t));
      return getter1::get(t);
    }


    static auto get_name() {
      return name_getter::get_name();
    }

    template <typename T>
    constexpr auto operator=(T t) {
      return ax_type<_Remove_cvref_t<T>, ax_name_container> {std::move(t)};
    }
    template <typename Data_T>
    using base_t = typename decltype(struct_maker123<Data_T>())::type;
  };

  template <typename ... Ts>
  ax_name_container(Ts&& ... ts) -> ax_name_container< _Remove_cvref_t<Ts>... >;



  template <typename T2>
  auto constexpr get_ax_name_container(const ax_name_container<T2>& t) {
    return  ax_name_container<T2>{};
  }



  template <typename T, typename data_T>
  struct base_maker
  {
    using type = typename  T::template base_t<data_T >;
  };
  template <typename T, typename data_T>
  using base_maker_t = typename base_maker<T, data_T>::type;



  template <typename... T>
  struct ntuple : base_maker_t<_Remove_cvref_t<T>, T>... {

    template<typename... Ts>
    ntuple(Ts&&... t1) : base_maker_t<_Remove_cvref_t<T>, T>(std::forward<Ts>(t1)) ...  {}


    template <typename T2>
    decltype(auto) operator[](const ax_name_container<T2>& t) {
      return  ax_name_container<T2>::get(*this);
    }



    friend std::ostream& operator<<(std::ostream& out, const ntuple& self) {
      out << "|";
      

      constexpr_for<0, sizeof...(T), 1>([&](auto i) {
        using  current_t = decltype(NthTypeOf<i, _Remove_cvref_t<T>...>{});
        out << " ";
        out << current_t::get(self);
        out << " |";
        });

      return out;
    }
  };

  template <typename ... Ts>
  ntuple(Ts&& ... ts) -> ntuple< _Remove_cvref_t<Ts> ...>;





  template <typename... Ts>
  struct dataframe : nt::base_maker_t<nt::_Remove_cvref_t<Ts>, nt::ax_type2< std::vector<Ts>, typename Ts::struct_maker> >... {

  template <typename T2>
  decltype(auto) operator[](const nt::ax_name_container<T2>& t) {
    return  nt::ax_name_container<T2>::get(*this);
  }

  auto operator[](size_t i) {
    using ret_t = nt::ntuple<_Remove_cvref_t<Ts>&...>;
    return ret_t{
        get<Ts>()[i]...
    };
  }

  auto operator[](size_t i)  const {
    using ret_t = nt::ntuple<const Ts&...>;
    return ret_t{
        get<Ts>()[i]...
    };
  }

  template <typename T>
  void push_back(const T& t) {

    [](auto...) {}
    ( Ts::get(*this).emplace_back(Ts::get(t)) ...);

  }

  template <typename T1>
  decltype(auto) get() const {
    return  T1::get(*this);
  }

  template <typename T1>
  decltype(auto) get() {
    return   T1::get(*this);
  }

  auto size() const {
    auto size = _Remove_cvref_t<NthTypeOf<0, Ts ...>>::get(*this).size();
    return size;
  }

  template <int N>
  static constexpr auto get_nth_type() {
    
    return get_ax_name_container(NthTypeOf<N, _Remove_cvref_t<Ts>...>{});
  }
  friend std::ostream& operator<<(std::ostream& out, const dataframe& self) {
    out << "|";
    
    constexpr_for<0, sizeof...(Ts), 1>([&](auto i) {
      static const auto x = self.get_nth_type<i>();
      out << " ";
      out <<  std::setw(5) << x.get_name();
      out << " |";
      });

    out << "\n";
    out << "|";
    constexpr_for<0, sizeof...(Ts), 1>([&](auto i) {
      
      out <<  std::setw(5) << "-------|";
      
      });
    out << "\n";
    auto size = self.size();
    for (int i = 0; i < size; ++i) {
      auto current_element = self[i];
      out << "|";
      constexpr_for<0, sizeof...(Ts), 1>([&](auto i) {
        static const  auto x = self.get_nth_type<i>();
        out << " ";
        out <<  std::setw(5) << x.get(current_element).v;
        out << " |";
        });
      out << "\n";

    }

    return out;
  }
};

template <typename ... Ts>
dataframe(Ts&& ... ts) -> dataframe< _Remove_cvref_t<Ts> ... >;


template <typename T>
struct dataframe_maker {


};

template <typename... T>
struct dataframe_maker<ntuple<T...>> {
  using type = dataframe<_Remove_cvref_t<T>...>;

};

template <typename F>
auto fill_dataframe(int index, F&& f) {
  typename dataframe_maker<decltype(f(0))>::type ret;
  for (int i = 0; i < index; ++i) {
    ret.push_back(f(i));
  }
  return ret;
}


}




#define ax_maker(name_)  [] { \
    auto struct_maker_template_lambda = [](auto e) { \
      if constexpr (!std::is_reference_v< decltype(e.val)> ){\
        struct Zt##name_ {\
         Zt##name_() {} \
         Zt##name_( const decltype(e.val) & e_): name_(e_) {} \
         Zt##name_(decltype(e.val) & e_): name_(e_) {} \
         Zt##name_(decltype(e.val) && e_): name_(std::move( e_ )) {} \
          decltype(e.val) name_; \
          decltype(e.val) value() const {\
            return name_;\
          }\
        }; \
        return nt::type_container<Zt##name_> {};\
      } else { \
        struct Zt##name_ {\
          Zt##name_(decltype(e.val)  e_): name_(e_) {} \
          decltype(e.val) name_; \
          decltype(e.val) value() const {\
            return name_;\
          }\
        }; \
        return nt::type_container<Zt##name_> {};\
      } \
    };\
    auto getter = [](auto& e) { \
      struct getter_t {\
        static constexpr auto& get(decltype(e)& x) {\
          return x.name_ ;\
        }\
      }; \
      return getter_t{}; \
    };\
    struct name_getter_t {\
      static auto get_name() {\
        return #name_;\
      }\
    };\
    static constexpr auto x = nt::ax_name_container<\
                                nt::ax_name_container_base<decltype(struct_maker_template_lambda), \
                                decltype(getter), \
                                name_getter_t> >{}; \
    return x;\
}()


#define ax_new_axis(name_, value) auto __internal__##name_ = [] {return  ax_maker(name_) = value; };\
                                  static  const inline  auto name_ = decltype(__internal__##name_() ){value}


#define ax_new_axis_t(name_, value) auto __internal__##name_ = [] {return  ax_maker(name_) = value; }; using  name_ = decltype (__internal__##name_() ) 
                                  
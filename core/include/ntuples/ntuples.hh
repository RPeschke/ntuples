


#include <type_traits>
#include <string>
#include <optional>
#include <iostream>

#include <vector>



template <class _Ty>
using _Remove_cvref_t = std::remove_cv_t<std::remove_reference_t<_Ty>>;



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
  constexpr ax_type operator=(T1 t1) const { return ax_type(std::move(t1)); }
  constexpr ax_type operator=(T1 t1) { v = std::move(t1);  return *this; }
  using struct_maker = T2;
  using data_t = T1;

  operator T1() {
    return v;
  }

  static  const std::string& name() {
    static  std::string ret = struct_maker::get_name();
    return ret;
  }

  friend std::ostream& operator<< (std::ostream& out, const ax_type& self) {
    out << self.name() << " : " << self.v;
    return out;
  }

};





template <typename T1, typename T2>
struct ax_type2 : T1, T2 {

  constexpr ax_type2() = default;


  using struct_maker = T2;
  using data_t = T1;




};


template <typename T2, typename T3, typename T4>
struct ax_name_container {

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
    using   getter1 = decltype(std::declval<T3>()(t));
    return getter1::get(t);
  }

  template <typename T>
  static constexpr decltype(auto) get1(T& t) {
    using   getter1 = decltype(std::declval<T3>()(t));
    return getter1::get(t);
  }

  static auto get_name() {
    return T4::get_name();
  }
  using type = T2;
  using getter = T3;
  using name_getter = T4;
  template <typename T>
  constexpr auto operator=(T t) {
    return ax_type<_Remove_cvref_t<T>, ax_name_container> {std::move(t)};
  }
  template <typename Data_T>
  using base_t = typename decltype(struct_maker123<Data_T>())::type;
};

template <typename ... Ts>
ax_name_container(Ts&& ... ts) -> ax_name_container< _Remove_cvref_t<Ts>... >;

template<int N, typename... Ts> using NthTypeOf =
typename std::tuple_element<N, std::tuple<Ts...>>::type;

template <typename T2, typename T3, typename T4>
auto get_ax_name_container(const ax_name_container<T2, T3, T4>& t) {
  return  ax_name_container<T2, T3, T4>{};
}



template <typename T, typename data_T>
struct base_maker
{
  using type = typename  T::struct_maker::template base_t<data_T >;
};
template <typename T, typename data_T>
using base_maker_t = typename base_maker<T, data_T>::type;



template <typename... T>
struct ntuple : base_maker_t<_Remove_cvref_t<T>, T>... {

  template<typename... Ts>
  ntuple(Ts&&... t1) : base_maker_t<_Remove_cvref_t<T>, T>(std::forward<Ts>(t1)) ...  {}


  template <typename T2, typename T3, typename T4>
  decltype(auto) operator[](const ax_name_container<T2, T3, T4>& t) {
    return  ax_name_container<T2, T3, T4>::get(*this);
  }



  friend std::ostream& operator<<(std::ostream& out, const ntuple& self) {
    out << "|";
    using current_t = NthTypeOf<0, T...>;

    constexpr_for<0, sizeof...(T), 1>([&](auto i) {
      using  current_t = decltype(get_ax_name_container(NthTypeOf<i, _Remove_cvref_t<T>...>{}));
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
struct dataframe : base_maker_t<_Remove_cvref_t<Ts>, ax_type2< std::vector<Ts>, typename Ts::struct_maker> >... {

  template <typename T2, typename T3, typename T4>
  decltype(auto) operator[](const ax_name_container<T2, T3, T4>& t) {
    return  ax_name_container<T2, T3, T4>::get(*this);
  }

  auto operator[](size_t i) {
    using ret_t = ntuple<_Remove_cvref_t<Ts>&...>;
    return ret_t{
        get<Ts>()[i]...
    };
  }

  auto operator[](size_t i)  const {
    using ret_t = ntuple<const Ts&...>;
    return ret_t{
        get<Ts>()[i]...
    };
  }

  template <typename T>
  void push_back(const T& t) {

    [](auto...) {}
    (_Remove_cvref_t<Ts>::struct_maker::get(*this).emplace_back(_Remove_cvref_t<Ts>::struct_maker::get(t))...);

  }

  template <typename T1>
  decltype(auto) get() const {
    return   _Remove_cvref_t<T1>::struct_maker::get(*this);
  }

  template <typename T1>
  decltype(auto) get() {
    return   _Remove_cvref_t<T1>::struct_maker::get(*this);
  }

  auto size() const {
    auto size = _Remove_cvref_t<NthTypeOf<0, Ts ...>>::struct_maker::get(*this).size();
    return size;
  }

  friend std::ostream& operator<<(std::ostream& out, const dataframe& self) {
    out << "|";
    constexpr_for<0, sizeof...(Ts), 1>([&](auto i) {

      using  current_t = decltype(get_ax_name_container(NthTypeOf<i, _Remove_cvref_t<Ts>...>{}));
      out << " ";
      out << current_t::get_name();
      out << " |";
      });
    out << "\n";

    auto size = self.size();
    for (int i = 0; i < size; ++i) {
      auto current_element = self[i];
      out << "|";
      constexpr_for<0, sizeof...(Ts), 1>([&](auto i) {
        using  current_t = decltype(get_ax_name_container(NthTypeOf<i, _Remove_cvref_t<Ts>...>{}));

        out << " ";
        out << current_t::get(current_element).v;
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







#define ax_maker(name_)  [] { \
auto l = [](auto e) { \
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
    return type_container<Zt##name_> {};\
  } else { \
    struct Zt##name_ {\
      Zt##name_(decltype(e.val)  e_): name_(e_) {} \
      decltype(e.val) name_; \
      decltype(e.val) value() const {\
        return name_;\
      }\
    }; \
    return type_container<Zt##name_> {};\
  } };\
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
constexpr auto x = ax_name_container<decltype(l), decltype(getter), name_getter_t>{}; return x;\
}()


#define ax_new_axis(name_, value) static constexpr auto name_ = (ax_maker(name_) = value)


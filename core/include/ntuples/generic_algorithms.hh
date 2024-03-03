#pragma once


namespace nt::algorithms
{

  template <typename FUNC_T>
  auto fill_vector(size_t entries, FUNC_T &&func)
  {
    std::vector<decltype(func(size_t(0)))> ret;
    ret.reserve(entries);
    for (size_t i = 0; i < entries; ++i)
    {
      ret.push_back(func(i));
    }
    return ret;
  }

  template <typename T0, typename T1, typename T2, typename Comparision_T, typename projecttion_t>
  void join_vectors_r(T0 &ret, const T1 &t1, const T2 &t2, Comparision_T comp, projecttion_t project)
  {
    ret.clear();
    for (const auto &e1 : t1)
    {
      for (const auto &e2 : t2)
      {
        if (comp(e1, e2))
        {
          ret.push_back(project(e1, e2));
        }
      }
    }
  }

  template <typename T1, typename T2, typename Comparision_T, typename projecttion_t>
  auto join_vectors(const T1 &t1, const T2 &t2, Comparision_T comp, projecttion_t project)
  {
    std::vector<decltype(project(t1[0], t2[0]))> ret;
    join_vectors_r(ret, t1, t2, comp, project);
    return ret;
  }

  template <typename CONTAINER_T>
  void sort(CONTAINER_T& container)
  {
    std::sort(container.begin(), container.end());
  }


  template <typename CONTAINER_T, typename OP_T>
  auto count_if(const  CONTAINER_T& container, OP_T op)
  {
    int i = 0;
    for (const auto& e : container)
      if (op(e))
        ++i;
    return i;
  }

}
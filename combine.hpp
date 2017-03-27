#if __cplusplus >= 201402L // C++14
namespace calls
{
    template<typename L1>
    struct Combined1 : L1
    {
        Combined1(L1 l1) : L1( std::move(l1) ) {}
        using L1::operator();
    };

    template<typename L1, typename L2>
    struct Combined2 : L1, L2
    {
        Combined2(L1 l1, L2 l2) :   L1( std::move(l1) ), L2( std::move(l2) ) {}

        using L1::operator();
        using L2::operator();
    };


    template<typename Ret, typename ...Args>
    inline Combined1<std::function<Ret(Args...)> > create(Ret(*f)(Args...))
    {
        return Combined1<std::function<Ret(Args...)> >(f);
    }

    template<typename Ret, typename ...Args>
    inline Combined1<std::function<Ret(Args...)> > create(std::function<Ret(Args...)> f)
    {
        return Combined1<std::function<Ret(Args...)> >(f);
    }


    template<typename L1>
    inline Combined1<L1> create(L1 l1)
    {
        return Combined1<L1>(l1);
    }

    template<typename L1, typename L2>
    inline Combined2<L1,L2> marge(const L1 &l1, const L2 &l2)
    {
        return Combined2<L1,L2>(l1,l2);
    }
}


template<typename C1_L1, typename C2_L1>
auto operator+(const calls::Combined1<C1_L1> &cb1, const calls::Combined1<C2_L1> &cb2)
{
    return calls::marge(cb1, cb2);
}

template<typename C1_L1, typename C2_L1, typename C2_L2>
auto operator+(const calls::Combined1<C1_L1> &cb1, const calls::Combined2<C2_L1, C2_L2> &cb2)
{
    return calls::marge(cb1, cb2);
}

template<typename C1_L1, typename C2_L1, typename C2_L2>
auto operator+(const calls::Combined2<C2_L1, C2_L2> &cb2,const calls::Combined1<C1_L1> &cb1)
{
    return calls::marge(cb1, cb2);
}

template<typename C1_L1, typename C1_L2, typename C2_L1, typename C2_L2>
auto operator+(const calls::Combined2<C1_L1, C1_L2> &cb1, const calls::Combined2<C2_L1, C2_L2> &cb2)
{
    return calls::marge(cb1, cb2);
}

template<typename C1_L1, typename Callable>
auto operator+(const calls::Combined1<C1_L1> &&cb1,const Callable &rhs)
{
    return calls::marge(cb1, calls::create(rhs));
}

template<typename C1_L1, typename C1_L2, typename Callable>
auto operator+(const calls::Combined2<C1_L1, C1_L2> &&cb1,const Callable &rhs)
{
    return calls::marge(cb1, calls::create(rhs));
}

template<typename RetType, typename ...Args, typename Callable>
auto operator+(RetType(*f)(Args...), const Callable &rhs)
{
    return calls::marge(calls::create(f), calls::create(rhs));
}

template<typename RetType, typename ...Args, typename Callable>
auto operator+(const std::function<RetType(Args...)> &f, const Callable &rhs)
{
    return calls::marge(calls::create(f), calls::create(rhs));
}

#else
    #error You need C++14 or higher compiler to use combine.hpp
#endif

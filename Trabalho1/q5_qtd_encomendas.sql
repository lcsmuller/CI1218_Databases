select n_name, count(o_orderkey)
    from NATION, ORDERS, CUSTOMER
    where o_custkey == c_custkey
        and c_nationkey == n_nationkey
    group by n_nationkey
    order by 2 desc;

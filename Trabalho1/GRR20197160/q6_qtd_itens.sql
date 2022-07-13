select n_name, sum(l_quantity)
    from NATION, ORDERS, LINEITEM, CUSTOMER
    where l_orderkey == o_orderkey
        and o_custkey == c_custkey
        and c_nationkey == n_nationkey
    group by n_nationkey
    order by 2 desc
    limit 10;

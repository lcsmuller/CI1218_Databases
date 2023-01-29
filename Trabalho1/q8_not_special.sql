select count(*)
    from (
        select distinct(o_custkey)
            from ORDERS 
        except
        select distinct(o_custkey)
            from ORDERS
            where o_comment like '%special request%'
        );

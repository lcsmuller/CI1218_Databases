select count(*)
    from (
        select c_custkey
            from CUSTOMER
        except
        select distinct(o_custkey)
            from ORDERS
        );

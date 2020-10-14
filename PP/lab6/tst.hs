--func list1 list2 = if (length list1) == 2 then list1 else list2
--func2 x = 1 : map (*2) x

--el1 lst = filter ((==0) . (`mod` 2)) lst
--el2 lst = filter ((/=0) . (`mod` 2)) lst
--el4 lst = filter (\x -> not $ mod x 2) lst

func1 l = (helper l) `div` (length l)
            where helper = foldr (+) 0

func2 l = (helper1 l) / (helper2 l)
            where 
                helper1 x = foldr (+) 0 x
                helper2 x 
                    | null x = 0
                    | otherwise = 1 + helper2 (tail x)

--f x y z = x y . z
--f x y = x (f y x)
--f x y z = foldr (.) id [x y z]
--f x y z = [ x ++ (y:z)]
--f x y = [(x, y), (y,x)]
--f x y = x y (y x)
--f x = [(z, z) | (y, z, t) <- x, y == z]

----------------------------------------------------------------------------------
--FLUXURI si ceva implementari
--[take 5 [m | m <- [n..], mod m n == 0] | n <- [1..]]
--[(a, b) | a <- [1, 2, 3], b <- [4, 5]] --cartezian
--take 10 $ zipWith (+) [0..] [1, 1..] == take 10 $ tail [0 ..] --egale pt [0..]
-- [x*x*x*x | x <- [1..]] --puterile lui 4
{-
perfect :: Integral a => a -> Bool
perfect n = n == sum [i | i <- [1..n-1], n `mod` i == 0]
filter perfect [1..]
-} --fluxul numerelor perfecte
--[take n $ [0 ..] | n <- [1..]] --[[], [0]. [0,1], [0, 1, 2] ...]
--foldl (\y _ -> y + 1 ) 0 [1] --length
--take 10 $ [ f a | a <- [0 ..], f <- [(+ 2)]] --aplica o functie f succesiv
{-
myConcat :: [a] -> [a] -> [a]
myConcat l1 l2 = foldr (:) l2 l1
-} --concatenare

{-
exists :: Eq a => a -> [a] -> Bool
exists element = foldr (||) False . map (== element)
-} -- lookup value in infinite stream

----------------------------------------------------------------------------------
--PROBLEME

--MultiMap 2019-A
data MultiMap key val = MultiMap [(key, [val])] deriving (Eq, Show)

ins :: Eq key => key -> val -> MultiMap key val -> MultiMap key val
ins k a (MultiMap lst) = MultiMap $ case back of
    [] -> (k, [a]) : front
    (_, as) : back -> (k, a : as) : front ++ back
    where
        (front, back) = break ((==k).fst) lst

map' ::  (a -> b) -> MultiMap k a -> MultiMap k b
map' f (MultiMap lst) = MultiMap $ map (\(k, as) -> (k, map f as)) lst

test = ins 2 'z' $ ins 3 'c' $ ins 2 'b' $ ins 1 'a' $ MultiMap []

--hashSet 2019-B
data HashSet hash val = HashSet [(hash, [val])] deriving (Eq, Show)

ins2 :: Eq hash => hash -> val -> HashSet hash val -> HashSet hash val
ins2 k a (HashSet lst) = HashSet $ case back of
    [] -> (k, [a]) : front
    (_, as) : back -> (k, a : as) : front ++ back
    where
        (front, back) = break ((==k).fst) lst

map'' :: (a->b) -> HashSet h a -> HashSet h b
map'' f (HashSet pairs) = HashSet $ map(\(h, as) -> (h, map f as)) pairs

test2 = ins 5 $ ins 2 $ ins 4 $ ins 8 $ HashSet []

--coada
data Queue a = Q [a] [a] deriving Show

isEmpty :: (Queue a) -> Bool
isEmpty (Q [] []) = True
isEmpty (Q _ _) = False

setDifference :: [Int] -> [Int] -> [Int]
setDifference a b = [x | x <- a, not $ elem x b]
                              
quicksort :: Ord a => [a] -> [a]
quicksort [] = []
quicksort(e:es) = quicksort smaller ++ [e] ++ quicksort bigger
    where bigger = [b | b <- es, b>e]		
          smaller = [a | a <- es, a<e]

--top :: (Queue a) -> 
--MList 2019-C
data MList a = I Int | C Char | P (Int, Char)
{-
filter' :: Char -> MList -> MList
filter' chr (MList val) = MList $ filter (f chr)
    where f 'i' (I _) = True
          f 'c' (C _) = True
          f 'p' (P _) = True
          f _ _ = False

conv :: MList -> Maybe [Char]
conv (MList ((C c):rest)) = 
    case (conv (MList rest)) of 
        Just l -> Just (c:l)
        Nothing -> Nothing

conv (M []) = Just []
conv _ = Nothing
-}


{-
----------------------------------------------------------------------------------
--INSTANTIERI
instance Num a => Summable [a] where
    t list = foldl (+) 0 list

data MyList a = MyList [[a]]

instance Num a => Summable MyList where
    t list = foldl (+) 0 (map (\subL -> (foldl (+) 0 subL)) list)

data Mytype a = Build a a
instance Show (Mytype a) where
    show (Build x y) = show x ++ show y

instance Number a => Show (a -> b) where
    show f = f 1

instance Show (f a -> b -> c) where
    show _ = "cool_function"

instance Num Bool where
    (+) a b = a || b
    (*) a b = a && b

class MyClass c where
    f :: c a -> a

data Natural = Zero | Succ Natural

instance Show Natural where
    show = show.val where
        val Zero = 0
        val (Succ n) = 1 + (val n)

instance Eq Natural where
    Zero == Zero = True
    Zero == (Succ _) = False
    (Succ _) == Zero = False
    (Succ n1) == (Succ n2) = n1 == n2

instance Num a => Ord [a] where
    l1 <= l2 = sum1 <= sum2 where
        sum1 = foldl (+) 0 l1
        sum2 = foldl (+) 0 l2

data Maybe a = Just a | Nothing
instance Num a => Num (Maybe a) where
    Nothing + Nothing = Nothing
    (Just _ ) + Nothing = Nothing
    Nothing + (Just _ ) = Nothing
    (Just n1 ) + (Just n2) = (Just (n1 + n2))

data Tree a = Node
    { key :: a
    , children :: [Tree a]
    } deriving (Eq, Show)

filterTree :: (a -> Bool) -> Tree a -> Tree a
filterTree p (Node k ch) = Node k $ map (filterTree p) $ filter (p.key) ch

instance (Num a, Num b) => Num (a, b) where
    (a, b) + (c, d) = ((head n1) + (head n2)) , (snd n1) + (snd n2))

instance (Num a, Num b, Show b) => Show (a -> b) where
    show f = concatMap (show .  f) [1..10]
-}
{-
data Student = Student String [(String, Int)] deriving Show

instance Eq Student where
    (Student _ [("PP", materie)]) == (Student _ [("PP", materie2)]) =  resolution where
        resolution    
            | (materie < 5) && (materie2) < 5 = True
            | (materie > 5) && (materie2) > 5 = True
            | (materie == 10) && (materie2 == 10) = True
            | otherwise = False
-}  
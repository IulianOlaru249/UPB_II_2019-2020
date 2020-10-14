length' :: (Num b) => [a] -> b
length' [] = 0
length' (_:xs) = 1 + (length' xs)

sum' :: (Num a) => [a] -> a
sum' [] = 0
sum' (a : xs) = a + (sum' xs)

initials :: String -> String -> String
initials first_name last_name = [f] ++ "." ++ [l] ++ "."
    where (f : _) = first_name
          (l : _) = last_name

bmis :: (RealFloat a) => [(a, a)] -> [a]
bmis people = [ bmi weight height | (weight, height) <- people]
    where bmi weight height = (weight / height ^ 2)


bmis2 :: (RealFloat a) => [(a, a)] -> [a]
bmis2 people = [ bmi | (weight, height) <- people, let bmi = weight / height ^ 2]

maximum' :: (Ord a) => [a] -> a
maximum' [] = error "Empty!"
maximum' [x] = x
maximum' (x:xs)
    | x > maxTail = x 
    | otherwise = maxTail
    where maxTail = maximum' xs

{-
data A = MakeA String deriving (Show, Eq, Ord)
data B = MakeB String deriving Show
data C = MakeC String deriving (Show, Eq)
-}

f (Nothing:xs) = f xs

data A = MakeA Int Float deriving Show
newtype B = MakeB A deriving Show

data Human = Person Int String deriving Show
person = Person 1

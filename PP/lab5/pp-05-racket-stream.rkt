#lang racket
; Mihnea Muraru & Andrei Olaru

;(display "----- Operatori pe fluxuri -----\n")

(define-syntax-rule (pack expr)
  ;(lambda () expr)) ; închideri
  (delay expr)) ; promisiuni

(define unpack
  ;(λ (package) (package))) ; închideri
  force) ; promisiuni



; =====================================
(define-syntax-rule (stream-cons h t)
  ;; define-syntax-rule definește o construcție nestrictă,
  ;; deci apelul stream-cons nu va evalua h și t
  (cons h (pack t)))

(define stream-car car)

(define (stream-cdr s)
  (unpack (cdr s)))

(define stream-nil '())

(define stream-null? null?)
; ===============================

(define (stream-take s n)
  (cond ((zero? n) '())
        ((stream-null? s) '())
        (else (cons (stream-car s) (stream-take (stream-cdr s) (- n 1))))))

(define (stream-drop s n)
  (cond ((zero? n) s)
        ((stream-null? s) s)
        (else (stream-drop (stream-cdr s) (- n 1)))))

(define (stream-map f s)
  (if (stream-null? s) s
      (stream-cons (f (stream-car s))
                   (stream-map f (stream-cdr s)))))

(define (stream-filter f? s)
  (cond ((stream-null? s) s)
        ((f? (stream-car s)) (stream-cons (stream-car s) (stream-filter f? (stream-cdr s))))
        (else (stream-filter f? (stream-cdr s)))))

(define (stream-zip-with f s1 s2)
  (stream-cons (f (stream-car s1) (stream-car s2))
               (stream-zip-with f (stream-cdr s1) (stream-cdr s2))))

(define (stream-append s1 s2)
  (if (stream-null? s1) s2
      (stream-cons (stream-car s1)
                   (stream-append (stream-cdr s1) s2))))

(define (stream-assoc k s)
  (cond
    ((stream-null? s) #f)
    ((equal? (car (stream-car s)) k) (stream-car s))
    (else (stream-assoc k (stream-cdr s)))))

(define (list->stream L)
  (if (null? L) stream-nil
      (stream-cons (car L) (list->stream (cdr L)))))

; ===============================================
(display "----- Definiri de fluxuri -----\n")


(display "ones:\n")

;(define ones (letrec ((ones (cons 1 ones))) ones)) ; eroare
;(define (ones) (cons 1 (ones)))  ;ciclu infinit
;(define ones (cons 1 (λ () ones))) ; inchideri
;(define ones (cons 1 (delay ones))) ; promisiuni
(define ones (stream-cons 1 ones))
; ATENȚIE: există și în racket definit stream-cons & co. Aici le-am suprascris.
; stream-cons din Racket întoarce un obiect de tip #<stream>

(car ones)
(cdr ones)
(equal? ones (stream-cdr ones))


; cu funcție recursivă
(define (naturalsFrom start) 
  (stream-cons start
               (naturalsFrom (+ start 1))))
(define naturals (naturalsFrom 0))

; cu funcție recursivă în named let
(define naturals2
  (let build ((n 0))
    (stream-cons n (build (add1 n)))))

(define naturals3
  (stream-cons 0 (stream-zip-with + ones naturals3)))

(display "naturals:\n")
(stream-take naturals 10)
(stream-take naturals2 10)
(stream-take naturals3 10)

; folosiți această definiție pentru a observa cum se construiesc elementele fluxului
(define naturalsBuild
  (let build ((start 0)) ; observați evaluarea când folosim promisiuni pentru întârziere
    (stream-cons
     start
     (build
      (and (display `(build ,(+ start 1))) (+ start 1))))))

(display "naturals building:\n")
(stream-take naturalsBuild 0)
(stream-take naturalsBuild 1) ; se construiește un element în față din cauza implementării lui stream-take:
(stream-take naturalsBuild 5) ; se apelează stream-take recursiv, care evaluează stream-cdr
(stream-take naturalsBuild 7)


(define even-naturals (stream-filter even? naturals))
(display "even naturals: ")
(stream-take even-naturals 5)

(define even-naturals2 (stream-zip-with + naturals naturals))
(stream-take even-naturals 5)


(define powers-of-2
  ;; definiție recursivă (explicită)
  (letrec ((build (λ (start)
                    (stream-cons start
                                 (build (* start 2))))))
    (build 1)))

(define p-o-2
  ;; variantă de definire implicită
  (stream-cons 1 (stream-map ((curry *) 2) p-o-2)))

(define p-o-2B
  ;; altă variantă de definire implicită
  (stream-cons 1 (stream-zip-with + p-o-2 p-o-2)))

(display "Powers of 2: ")
(stream-take p-o-2 12)

(define primes ;; primes
  (letrec ((sieve
            (λ (numbers)
              (let ((prime (stream-car numbers)))
                (stream-cons prime
                             (sieve (stream-filter
                                     (λ (n) (not (zero? (remainder n prime))))
                                     (stream-cdr numbers)))
                             )))))
    (sieve (naturalsFrom 2))))

(display "primes: ")
(stream-take primes 10)

(define my-stream (stream-cons naturals
                              my-stream))
                              
(stream-first
 (stream-map
  (lambda (stream) (stream-filter even?
                                  stream)) my-stream))

(display "1000th prime: ")
(stream-car (stream-drop primes 999))



; =============================================== LAZY BFS
(display "----- Căutare leneşă în spaţiul stărilor -----\n")


;(display "Găsirea primei stări scop\n")

(define symbols '(a b c)) ;; alfabetul pentru palindroame
(define expand-string ;; pe baza lui s, mai construiește o serie de șiruri, de lungime + 1
  (λ (s) (map (λ (symb) (cons symb s)) symbols)))
(define palindrome? (λ (s) (equal? s (reverse s))))
(define (n-palindrome? n) (λ (s) (and (equal? n (length s)) (palindrome? s)))) ; curry

(define BFS-one ;; găsește prima soluție și o întoarce
  (λ (init expand goal?)
    (let search ((frontier (list init))) ;; stări de explorat
      ;(display frontiera)
      (if (null? frontier) #f  ;; am terminat de explorat și nu am găsit soluție
          (let ((node (car frontier))) ;; stare curentă
            (if (goal? node)
                node ;; am găsit o stare scop (o soluție)
                (search (append (cdr frontier) (expand node))))
            ;; stările nou descoperite se adaugă la sfârșit
            )))
    ))
;; primul palindrom de lungime 3:
(BFS-one '() expand-string (n-palindrome? 3))


(define BFS ;; găsește toate soluțiile (spațiul de căutare trebuie să fie finit)
  (λ (init expand goal?)
    (let search ((frontier (list init))) ;; stări de explorat
      ;(display frontiera)
      (if (null? frontier) '()  ;; am terminat de explorat?
          (let* ((node (car frontier)) ; stare curentă
                 (other-solutions ;; rezultatul căutării recursive
                  (search (append (cdr frontier) (expand node)))))
            ;; stările nou descoperite se adaugă la sfârșit
            (if (goal? node)
                (cons node other-solutions) ;; am găsit o stare scop (o soluție)
                other-solutions)
            )))
    ))

(define limited-expand-string ; limitează rezultatul lui expand-string la lungimea len
  (λ (len) (λ (s) ; definiție curry
             (filter (λ (x) (<= (length x) 3)) (expand-string s)))))
; alternativ:
;(define limited-expand-string (compose ((curry filter) (compose ((curry >=) 3) length)) expand-string))

(define 3-palindromes (BFS '()
                           (limited-expand-string 3)
                           (n-palindrome? 3)))
(display "palindroame de lungime 3: ")
3-palindromes


(define lazy-BFS ;; produce un flux de soluții, pe baza unui spațiu de căutare potențial infinit
  (λ (init expand goal?)
    (let search ((frontier (list init))) ;; stări de explorat
      ;(display frontiera)
      (if (null? frontier) stream-nil  ;; am terminat de explorat?
          (let ((node (car frontier))) ;; stare curentă
            (if (goal? node)

                ;; la găsirea unei stări scop, restul căutării va fi lăsat
                ;; până când este necesară o nouă soluție (ceea ce va forța evaluarea lui cdr din
                ;; fluxul de stări, și deci evaluarea lui search)
                (stream-cons node (search (append (cdr frontier) (expand node))))

                ;; dacă nod nu este scop, continuăm căutarea pentru a putea livra un nod scop
                (search (append (cdr frontier) (expand node)))
                )))
      )))


(define all-palindromes
  (lazy-BFS '()
            expand-string
            palindrome?
            ))
(displayln "palindroamele de la 50 la 60:")
(stream-take (stream-drop all-palindromes 50) 10)


(define levels-stream
  (stream-cons '(()) ; primul nivel conține doar șirul gol
               (stream-map (compose
                            ((curry apply) append) ; facem flatten după ce
                            ((curry map) expand-string)) ; am expandat întreg nivelul
                           levels-stream)))            ; și formăm un nou nivel
;(stream-take levels-stream 3)

(define search-space ; forma flat pentru niveluri
  (let build ((cLevel '()) (levels levels-stream))
    (if (null? cLevel) (build (stream-car levels) (stream-cdr levels))
        (stream-cons (car cLevel) (build (cdr cLevel) levels)))))
;(stream-take search-space 20)

(define lean-palindromes (stream-filter palindrome? search-space))
(displayln "palindroamele de la 50 la 60:")
(stream-take (stream-drop lean-palindromes 50) 10)

;
;
;
;
;(define breadth-search-goal
;  (lambda (init expand goal?)
;    (letrec ((search
;              (lambda (states)
;                (if (null? states) '()
;                    (let ((state (car states))
;                          (states (cdr states)))
;                      (if (goal? state) state
;                          (search (append states (expand state)))))))))
;      (search (list init)))))
;
;
;;(display "Fluxul stărilor\n")
;
;(define lazy-breadth-search
;  (lambda (init expand)
;    (letrec ((search
;              (lambda (states)
;                (if (stream-null? states) states
;                    (let ((state (stream-car states))
;                          (states (stream-cdr states)))
;                      (stream-cons state (search (stream-append states (expand state)))))))))
;      (search (stream-cons init stream-nil)))))
;
;
;;(display "Fluxul stărilor goal\n")
;
;(define lazy-breadth-search-goal
;  (lambda (init expand goal?)
;    (stream-filter goal? (lazy-breadth-search init expand))))
;
;
;(display "Fluxul palindroamelor\n")
;
;(define palindromes
;  (lambda (n symbols)
;    (let ((symbol-stream (list->stream symbols)))
;      (lazy-breadth-search-goal '()
;                                (lambda (state)
;                                  (stream-map (lambda (symbol) (cons symbol state))
;                                              symbol-stream))
;                                (lambda (state)
;                                  (and (>= (length state) n) (equal? state (reverse state))))))))
;
;(stream-take (palindromes 2 '(a b)) 10)
;
;
;(display "Problema reginelor\n")
;
;; board -> '((1 . 2) (2 . 4) (3 . 1) (4 . 3))
;
;(define queens
;  (lambda (n)
;    (lazy-breadth-search-goal '()
;                              (expandq n)
;                              (lambda (board)
;                                (= (length board) n)))))
;
;(define lin car)
;(define col cdr)
;
;(define conflict?
;  (lambda (q1 q2)
;    (or (= (lin q1) (lin q2))
;        (= (col q1) (col q2))
;        (= (abs (- (lin q1) (lin q2)))
;           (abs (- (col q1) (col q2)))))))
;
;(define forall?
;  (lambda (p? l)
;    (foldl (lambda (x y) (and x y)) #t (map p? l))))
;
;(define expandq
;  (lambda (n)
;    (lambda (board)
;      (let ((line (+ (length board) 1))
;            (columns (let func ((n n))
;                       (if (zero? n) '()
;                           (cons n (func (- n 1)))))))
;        (if (> line n) '()
;            (map (lambda (column)
;                   (cons (cons line column) board))
;                 (filter (lambda (column)
;                           (forall? (lambda (queen)
;                                      (not (conflict? queen (cons line column))))
;                                    board))
;                         columns)))))))
;
;(stream-take (queens 8) 10)
;
;
;
;
;
;; =============================================== GRAPHS
;(display "Grafuri:\n")
;
;(define (make-directed-graph V E) (cons V E))
;(define get-nodes car)
;(define get-edges cdr)
;(define (outgoing G v) (map cadr (filter (λ (e) (equal? (car e) v)) (get-edges G))))
;
;
;(define (curry f NTotal) (let c-R ((N NTotal) (args '())) (if (zero? N) (apply f (reverse args))
;                                                              (λ (arg) (c-R (- N 1) (cons arg args))))))
;(define (in-sol n partSol) (ormap ((curry assoc 2) n) partSol))
;
;(define (dfs-one G v . listsolution)
;  (reverse (let dfs-R ((c-node v) (local-H (list (cons v 'none))))
;             (foldl (λ (child history)
;                      (if (or (assoc child history) (in-sol child (car listsolution)))
;                          history
;                          (dfs-R child (cons (cons child c-node) history))
;                          )) local-H (outgoing G c-node))
;             )))
;
;(define (dfs G)
;  (foldl (λ (node solution)
;           (if (in-sol node solution) solution
;               (append solution (list (dfs-one G node solution)))
;               )) '() (get-nodes G)))
;
;(define (dfs-C G v)
;  (reverse (let dfs-R ((c-node v) (solution (list (cons v 'none))))
;             (foldl (λ (child sol)
;                      (if (assoc child sol)
;                          sol
;                          (dfs-R child (cons (cons child c-node) sol))
;                          )) solution (outgoing G c-node))
;             )))
;
;
;(define G1 (make-directed-graph
;            '(0 1 2 3 4 5 7)
;            '((0 4) (4 3) (3 7) (3 2) (3 1) (7 2) (0 5) (3 5) (3 0) (5 4))))
;(dfs-C G1 0)
;
;
;
;(display "Graf infinit:\n")
;
;(random-seed 66)
;(define (Edges-gen)
;  (let e-g ((nodes (list 0)) (edges '()))
;    (let* ((source (list-ref nodes (random (length nodes))))
;           (span (apply max nodes))
;           (d-nodes (remove source (range (+ span 5))))
;           (destination (list-ref d-nodes (random (length d-nodes))))
;           (edge (list source destination))
;           )
;      (if (member edge edges)
;          (e-g nodes edges)
;          (stream-cons edge (e-g (if (member destination nodes) nodes (cons destination nodes))
;                                 (cons edge edges)))
;          ))))
;
;(define InfiniGraph (Edges-gen))
;
;(define (infini-get-nodes G)
;  (let get ((nodes '()) (edges G))
;    (let ((to-add (filter (λ (v) (not (member v nodes))) (stream-car edges))))
;      (cond
;        ((= (length to-add) 2) (stream-cons (car to-add) (stream-cons (cadr to-add) (get (append nodes to-add) (stream-cdr edges)))))
;        ((= (length to-add) 1) (stream-cons (car to-add) (get (append nodes to-add) (stream-cdr edges))))
;        ((= (length to-add) 0) (get (append nodes to-add) (stream-cdr edges)))
;        ))))
;(define infini-get-edges identity)
;
;
;
;(define (lazy-outgoing G v limited limit)
;  (stream-map cadr (stream-filter
;                    (λ (e) (and (equal? (car e) v) (member e limited)))
;                    (list->stream (stream-take G limit)))))
;
;(define (lazy-dfs-C G v limited limit)
;  (let dfs-R ((c-node v) (solution (list->stream (list (cons v 'none)))))
;    (let visit ((children (lazy-outgoing G c-node limited limit)) (sol solution))
;      ;(display (stream-take children 50)) (display " ")
;      ;(display (stream-take sol 50))
;      ;(newline)
;      (if (stream-null? children) sol
;          (visit (stream-cdr children)
;                 (let ((child (stream-car children)))
;                   (if (stream-assoc child sol) sol
;                       (dfs-R child (stream-append sol (stream-cons (cons child c-node) '()))))))))))
;
;(stream-take (infini-get-nodes InfiniGraph) 10)
;(stream-take (infini-get-edges InfiniGraph) 15)
;(stream-take (lazy-dfs-C InfiniGraph 0 (stream-take InfiniGraph 15) 1000) 10)
;
;
;;; old
;(define (lazy-dfs-one G v . listsolution)
;  (let dfs-R ((node v) 
;              (local-H (list->stream
;                        (list (cons v 'none))))
;              (MAXDepth -1))
;    (if (zero? MAXDepth) local-H
;        (let visit-children ((children (lazy-outgoing G node))
;                             (history local-H))
;          (if (stream-null? children) history
;              (let ((child (stream-car children)))       
;                (if (or (stream-assoc child history)
;                        (stream-assoc child (car listsolution)))
;                    history
;                    (dfs-R
;                     child
;                     (stream-append history (list->stream (list (cons child node))))
;                     (- MAXDepth 1)))
;                ))))))
;(define (lazy-dfs G)
;  (let lazy-dfs-R ((nodes (get-nodes G))
;                   (solution stream-nil))
;    (if (stream-assoc (stream-car nodes) solution)
;        solution
;        (lazy-dfs-R (stream-cdr nodes)
;                    (stream-append solution
;                                   (lazy-dfs-one G (stream-car nodes) solution))))
;    ))
;
;;(stream-take (lazy-dfs InfiniGraph) 10)

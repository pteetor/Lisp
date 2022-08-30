;;
;; Sandbox for Lisp in general and SBCL in particular
;;

(mapcar #'abs '(3 -4 2 -5 -6))

(defun dbl (x) (* 2 x))

(dbl 5)

(mapcar (function dbl) (mapcar #'dbl '(1 2 3 4)))

(function dbl)

(defun read-all (stream so-far)
  (let ((next (read stream nil nil)))
    (if (null next)
	so-far
	(read-all stream (append so-far next)) )))

;; ----------------------------------------------------------

(defstruct Instr
  "One instruction in the virtual machine"
  label
  opcode
  opand )

(defun polish (x)
  (if (atom x)
      (polish-atom x)
      (polish-fun-call x) ))

(defun polish-atom (x) (make-Instr :opcode 'PUSH :opand x))

(defun polish-fun-call (x)
  (let ((fn (car x))
	(args (cdr x))
	(nArgs (length (cdr x))))
    (cond
      ((eq fn 'if)
       (let ((if-cond (first args))
	     (then-clause (second args))
	     (else-clause (third args)) )
	 (list (polish if-cond)
	       (make-Instr :opcode 'BR-FALSE :opand 'X)
	       (polish then-clause)
	       (make-Instr :opcode 'BRANCH :opand 'X)
	       (polish else-clause) )))
      (t
       (append (mapcar #'polish args)
	       (list (polish fn)
		     (make-Instr :opcode 'CALL :opand nArgs) )))
      )
    )
  )


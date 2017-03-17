        SUBROUTINE SQMULT(A, B, C, n)
        DOUBLE PRECISION A(n,n), B(n,n), C(n,n)
        DO i = 1,n
          DO j = 1,n
            C(i,j) = 0
          ENDDO
        ENDDO

!	TODO: Implement the rest of matrix multiply

        END

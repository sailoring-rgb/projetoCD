import java.sql.SQLOutput;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

///////////////////////////////////////// VERSÃO 2 (EXERCÍCIO 1.2) /////////////////////////////////////////

/*
    IMPORTANTE !!!!!

    O await implicitamente já faz unlock. O lock é libertado atomicamente e a execução da thread é suspendida.

    A signalAll acorda todas as threads e estas competem pelo lock, ganhando a que se “despachar” primeiro. Ou seja,
    se sinalizou todas as threads, mesmo se não “ganharam” o lock em primeiro lugar, todas saem do estado wait-set e passam a esperar pelo lock.

    Para verificar as condições, precisamos sempre de um ciclo em vez de um if. Isto porquê? Às vezes há bugs no sistema que levam a que
    as threads saiam do await sem ter havido um signalAll. Utilizando um ciclo, o programa vai estar sempre a verificar se a thread está no await.
*/


class Barrier {

    // Precisamos de uma condição e, para termos uma condição, precisamos de um lock.
    private Lock lock = new ReentrantLock();
    private Condition cond = lock.newCondition();

    private int contador = 0;
    private int N;

    private int epoca;

    public Barrier(int N) {
        this.N = N;
        epoca = 0;
    }

    void await() throws InterruptedException {
        lock.lock();
        
        // Estamos na época 1. No entanto, começam a chegar threads relativas à época 2. Ora, estas têm de esperar.
        // Deste modo, primeiramente temos de saber em que época ele está.

        // O objetivo é que as threads da época atual sejam libertadas (para podermos passar à proxima época)
        // e que as threads que chegam (da próxima época) esperem.

        // Este método será feito numa época e, quando for chamado novamente, já estará na época a seguir.
        // Isto permite que a operação possa ser usada várias vezes por cada thread (barreira reutilizável).

        int epoca = this.epoca;                

        contador++;                                   // conta (mais) uma thread: a atual

        if (contador < N) {                           // se todas as N threads não o tiverem invocado
            while (epoca == this.epoca)               // e enquanto a época não transitar (ou seja, ainda estiver na minha época atual)
                cond.await();                         // as threads, tanto da época atual, como da época a seguir, têm de esperar.
        }
        else {
            cond.signalAll();                         // a última thread a chegar tem de fazer signalAll
            this.epoca++;                             // transita de época
            contador = 0;                             // e, para as threads que vêm a seguir (para a prox época), temos de reiniciar o processo
        }
        lock.unlock();
    }

}
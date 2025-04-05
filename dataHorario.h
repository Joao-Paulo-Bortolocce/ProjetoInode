#include <time.h>

TpData obterDataAtual() {
    time_t agora = time(NULL);
    struct tm *local = localtime(&agora);

    struct TpData data;
    data.dia = local->tm_mday;
    data.mes = local->tm_mon + 1;
    data.ano = local->tm_year + 1900;

    return data;
}

TpHorario obterHorarioAtual() {
    time_t agora = time(NULL);
    struct tm *local = localtime(&agora);

    struct TpHorario horario;
    horario.hora = local->tm_hour;
    horario.minuto = local->tm_min;
    horario.segundo = local->tm_sec;

    return horario;
}

namespace Chat
{
    void handle(int signal);
    void setHandler(int signal, void (*handler)(int));
}
/***************************************************************/
/* Computacao Grafica 2018.2 - Profa Dr. Regina Celia Coelho   */
/* Nome: Willian Dihanster Gomes de Oliveira RA: 112269        */
/* Projeto Final: Lara Croft World                             */
/***************************************************************/
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <GL/freeglut.h>

/*Definicao e inicializacao das variveis globais*/
GLint slices = 100, stacks = 100, nrings = 100, largura_tela = 800, altura_tela = 800;
GLint estado = 0, s = 0, andando = 0, parou_tudo = 0, esq = 0, cont = 0;
GLint flag_braco_esq = 0, flag_braco_dir = 0, flag_antebraco_esq = 0, flag_antebraco_dir;
GLint flag_perna_esq = 0, flag_perna_dir = 0, flag_joelho_esq = 0, flag_joelho_dir = 0;
GLint flag_esq = 0, flag_dir = 0, flag_visao = 1;
GLint flag_braco = 0, flag_antebraco = 0, flag_perna = 0, flag_joelho = 0;
GLint flag_cabeca = 0, flag_cena = 1, flag_animacao = 0;
GLint flag_mao_esq = 0, flag_mao_dir = 0;
GLuint texture_id;
GLfloat angle = 45.0;
GLfloat fAspect;
GLdouble x_pos = 0.0, y_pos = 0.0, x_mouse_novo, y_mouse_novo, x_mouse = 0.0, y_mouse = 0.0;
GLdouble ang_x = 0.0, ang_y = 0.0, ang_z = 0.0, escala = 0.2, rot_tronco = 0.0;
GLdouble rotacao = 0.0, velocidade_lara = 4.0;
GLdouble velocidade_rotacao = 4.0;
GLdouble rot_lanca = 0.0, y_lanca = 0.0;
GLdouble rot_braco_esq = 0.0, rot_braco_dir = 0.0, rot_mao_esq = 0.0, rot_mao_dir = 0.0;
GLdouble rot_ant_braco_esq = 0.0, rot_ant_braco_dir = 0.0;
GLdouble rot_perna_esq = 0.0, rot_perna_dir = 0.0, rot_joelho_esq = 0.0, rot_joelho_dir = 0.0;
GLdouble rot_pe_esq = 0.0, rot_pe_dir = 0, rot_cabeca = 0.0, soma_rot = 0;
GLdouble pan_x = 0.0, pan_y = 0.0, pan_z = 0.0;

/*Definicao dos pontos de controle da Spline*/
GLfloat ctrlpoints1[6][3] = {
    {-4.0, -4.0, 0.0}, {-2.0, 4.0, 0.0}, {2.0, -4.0, 0.0},
    {3.81, 7.8, 0.0}, {6.0, -2.0, 0.0}, {6.8, -4.0, 0}
};

/*Declaracao de algumas funcoes*/
void animacao();
void faz_opcao_menu();
void display();
void atualiza_janela(GLsizei w, GLsizei h);
static void funcoes_teclas(unsigned char key, int x, int y);
void menu_opcoes(GLint opcao);

/*Definicao da Estrutura pra o arquivo bmp*/
typedef struct BMPImagem {
    int width;
    int height;
    char *data;
}BMPImage;

/*Definicao do tipo de dados da fila*/
struct dados {
    int info;
    double x, y;
    struct dados *prox;
};typedef struct dados tipoDado;

/*Definicao do tipo da fila*/
struct fila {
    tipoDado *primeiro;
    tipoDado *ultimo;
    int tamanho;
};typedef struct fila tipoFila;

/*Inicia uma fila nula*/
tipoFila *filaAux = NULL;
/*Faz a insercao de um novo elemento da fila com coord x e y*/
void insereFinal(double x, double y){

    tipoDado *novo = (tipoDado*) malloc(sizeof(tipoDado));
    novo->x = x;
    novo->y = y;

    if(filaAux == NULL){
        filaAux = (tipoFila*) malloc(sizeof(tipoFila));
        novo->prox = NULL;
        filaAux->primeiro = novo;
        filaAux->ultimo = novo;
    }
    else{
        novo->prox = NULL;
        filaAux->ultimo->prox = novo;
        filaAux->ultimo = novo;
    }

    filaAux->tamanho++;

}

/*Faz a inicializacao das funcoes, modos, janelas e etc das funcoes da GL e GLUT*/
void inicializacao(){

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.1, 3000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glOrtho(-largura_tela/2 + pan_x, largura_tela/2 + pan_x, -altura_tela/2 + pan_y, altura_tela/2 + pan_y, 0 + pan_z, 1000 + pan_z);

    gluLookAt(120, -10, 15, 0, 0, 0, 0, 1, 0);

}

void desenha_cabeca(){

    //Tranca
    for (int i = 0; i < 13; i++){
        if (i == 0 || i == 11)
            glColor3d(1.0, 0.83, 0.53);
        else
            glColor3d(0.18, 0.11, 0.05);
        glPushMatrix();
            glTranslated(0.0, 30.0-i*5 - 4*i, -50.0);
            glutSolidSphere(5, slices, stacks);
        glPopMatrix();
    }

    //Couro cabeludo
    glColor3d(0.18, 0.11, 0.05);
    glPushMatrix();
        glTranslated(0.0, 35.0, -6.0);
        glutSolidSphere(40, slices, stacks);
    glPopMatrix();

    //Cabeca
    glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
    glPushMatrix();
        glTranslated(0.0, 30.0, 0.0);
        glutSolidSphere(40, slices, stacks);
    glPopMatrix();

    //Olhos
    glColor3d(1.0, 1.0, 1.0);
    glPushMatrix();
        glTranslated(12.0, 35.0, 28.0);
        glScaled(1.5, 0.8, 1.0);
        glutSolidSphere(11, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-12.0, 35.0, 28.0);
        glScaled(1.5, 0.8, 1.0);
        glutSolidSphere(11, slices, stacks);
    glPopMatrix();

    glColor3d(0.0, 0.0, 0.0);
    glPushMatrix();
        glTranslated(17.0, 35.0, 34.0);
        glScaled(1.3, 1.0, 1.0);
        glutSolidSphere(6, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-17.0, 35.0, 34.0);
        glScaled(1.3, 1.0, 1.0);
        glutSolidSphere(6, slices, stacks);
    glPopMatrix();

    //Boca
    glPushMatrix();
        glColor3d(1, 0, 0);
        glTranslated(0.0, 10.0, 25.0);
        glScaled(1.2, 0.7, 1.0);
        glutSolidSphere(10, slices, stacks);
    glPopMatrix();

    //Nariz
    glPushMatrix();
        glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
        glTranslated(0.0, 25.0, 35.0);
        glScaled(10, 10, 10);
        glutSolidTetrahedron();
    glPopMatrix();

    //Pescoco
    glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
    glPushMatrix();
        glTranslated(0, -4, 0);
        glScaled(0.7, 4.0, 0.6);
        glRotated(90, 1, 0, 0);
        glutSolidCylinder(20, 4, slices, stacks);
    glPopMatrix();

    //Oculos
    glColor3d(0.70, 0.22, 0.22);

    glPushMatrix();
        glTranslated(18.0, 35.0, 40.0);
        glScaled(0.5, 0.5, 0.1);
        glutSolidSphere(20, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-18.0, 35.0, 40.0);
        glScaled(0.5, 0.5, 0.1);
        glutSolidSphere(20, slices, stacks);
    glPopMatrix();

    glColor3d(0, 0, 0);
    glPushMatrix();
        glTranslated(0.0, 34.0, 35.0);
        glScaled(1.6, 0.2, 1.0);
        glutSolidCube(10);
    glPopMatrix();

    //Orelha
    glPushMatrix();
        glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
        glScaled(1.0, 1.0, 1.0);
        glTranslated(31, 30, 3);
        glutSolidSphere(10, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
        glScaled(1.0, 1.0, 1.0);
        glTranslated(-31, 30, 3);
        glutSolidSphere(10, slices, stacks);
    glPopMatrix();
}

void desenha_tronco(){

    //Seios
    glColor3d(0.38, 0.70, 0.60);
    /*//glColor3d(175.0/255.0, 238.0/255.0, 238/255.0);
    glPushMatrix();
        glTranslated(+11.0, -45.0, 17.0);
        glutSolidSphere(14, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-11.0, -45.0, 17.0);
        glutSolidSphere(14, slices, stacks);
    glPopMatrix();
    */

    //Torax
    glPushMatrix();
        glTranslated(0.0, -100.0, 0.0);
        glScaled(0.7, 1.7, 0.6);
        glRotated(180, 0, 0, 1);
        glRotated(90, 1, 0, 0);
        glutSolidCone(40, 50, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0.0, -50.0, 0.0);
        glScaled(1.0, 1.1, 0.9);
        glutSolidSphere(30, slices, stacks);
    glPopMatrix();

    //Mochila
    glColor3d(0.76, 0.45, 0.25);
    glPushMatrix();
        glColor3d(0.76, 0.45, 0.25);
        glTranslated(0.0, -45.0, -20.0);
        glScaled(1.2, 1.0, 0.7);
        glutSolidCube(40);
    glPopMatrix();

    //Alca da mochila
    glPushMatrix();
        glTranslated(-30.0, -40.0, 0.0);
        glScaled(0.9, 1.0, 1.2);
        glRotated(-20, 0, 0, 1);
        glRotated(90, 0, 1, 0);
        glutSolidTorus(3, 10, 100, 100);
    glPopMatrix();

    glPushMatrix();
        glTranslated(30.0, -40.0, 0.0);
        glScaled(0.9, 1.0, 1.2);
        glRotated(20, 0, 0, 1);
        glRotated(-90, 0, 1, 0);
        glutSolidTorus(3, 10, 100, 100);
    glPopMatrix();

}

void desenha_bracos(){

    glPushMatrix();
        glTranslated(0, -5, 0);

        //Braco Esquerdo
        glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
        glPushMatrix(); /*Hierarquia braco esquerdo*/
            glTranslated(-5.0, 0.0, -7.0);
            glScaled(1.0, 1.0, 0.7);

            glTranslated(-35, -30, 10);
            glRotated(rot_braco_esq, 1, 0, 0);
            glRotated(+5, 0, 0, 1);
            glTranslated(+35, +30, -10);

            glPushMatrix(); /*Braco comeco*/
                glTranslated(-35, -30, 10);
                glRotated(-15, 0, 0, 1);
                glScaled(1.0, 1.0, 1.4);
                glRotated(90, 1, 0, 0);
                glutSolidCylinder(8, 50, slices, stacks);
            glPopMatrix();

            glPushMatrix(); /*Braco ligamento redondo*/
                glTranslated(-34, -32, 10);
                glRotated(-rot_braco_esq, 1, 0, 0);
                glScaled(1.0, 1.0, 1.7);
                glutSolidSphere(10, slices, stacks);
            glPopMatrix();

            glPushMatrix(); /*Hierarquia antebraco*/
                glPushMatrix(); /*Ligamento braco-antebraco*/
                    glTranslated(-47, -75, 10);
                    glScaled(1.0, 1.0, 1.5);
                    glutSolidSphere(8.5, slices, stacks);
                glPopMatrix();

                glTranslated(-46, -75, 10);
                glRotated(-15, 0, 0, 1);
                glRotated(rot_ant_braco_esq, 1, 0, 0);
                glRotated(+15, 0, 0, 1);
                glTranslated(+46, +75, -10);

                glPushMatrix(); /*Desenha antebraco*/
                    glTranslated(-46, -75, 10);
                    glRotated(-15, 0, 0, 1);
                    glScaled(1.0, 1.0, 1.4);
                    glRotated(90, 1, 0, 0);
                    glutSolidCylinder(8, 25, slices, stacks);
                glPopMatrix();

                glPushMatrix(); /*Hierarquia mao*/

                        //glPushMatrix(); /*Ligamento antebraco e mao*/
                          /*  glColor3d(0, 0, 0);
                            glTranslated(-52, -97, 10);
                            glScaled(1.0, 1.0, 1.0);
                            glRotated(-15, 0, 0, 1);
                            glRotated(90, 1, 0, 0);
                            glutSolidCylinder(8, 10, slices, stacks);
                        glPopMatrix();*/

                        glTranslated(-53, -105, 10);
                        glRotated(-15, 0, 0, 1);
                        glRotated(rot_mao_esq, 1, 0, 0);
                        glRotated(+15, 0, 0, 1);
                        glTranslated(+53, +105, -10);

                        glPushMatrix();
                            glColor3d(0, 0, 0);
                            glTranslated(-53, -105, 10);
                            glRotated(-15, 0, 0, 1);
                            glScaled(1.0, 1.4, 1.4);
                            glRotated(90, 1, 0, 0);
                            glutSolidSphere(10, slices, stacks);
                        glPopMatrix();

                        //Mapa
                        if (flag_animacao == 1){
                            glPushMatrix();
                                glTranslated(-65, -110, 20);
                                glScaled(2.0, 2.0, 0.1);
                                glutSolidCube(25);
                            glPopMatrix();
                        }
                        //Dedo
                        /*
                        for (int i = 0; i < 5; i++){
                            glPushMatrix();
                                glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
                                glTranslated(+4 * i - 100, -98, 10);
                                glRotated(30, 0, 0, 1);
                                //glRotated(-30, 0, 0, 1);
                                glScaled(0.5, 1.9, 0.3);
                                //glRotated(90, 1, 0, 0);
                                glutSolidCylinder(4, 5, slices, stacks);
                            glPopMatrix();
                        }
                        */
                glPopMatrix(); /*Fim hierarquia mao*/

            glPopMatrix(); /*FIM Hierarquia antebraco*/

        glPopMatrix(); /*FIM Hierarquia braco esquerdo*/

    glPopMatrix();

    glPushMatrix();
        /*Braco Direito*/
        glTranslated(0, -5, 0);
        glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);

        glPushMatrix(); /*Hierarquia braco direito*/
            glTranslated(+5.0, 0.0, -7.0);
            //glTranslated(5.0, -2.0, -7.0);
            glScaled(1.0, 1.0, 0.7);

            glTranslated(+35, -30, 10);
            glRotated(rot_braco_dir, 1, 0, 0);
            glRotated(-5, 0, 0, 1);
            glTranslated(-35, +30, -10);

            glPushMatrix(); /*Braco comeco*/
                glTranslated(+35, -30, 10);
                glRotated(+15, 0, 0, 1);
                glScaled(1.0, 1.0, 1.4);
                glRotated(90, 1, 0, 0);
                glutSolidCylinder(8, 50, slices, stacks);
            glPopMatrix();

            glPushMatrix(); /*Braco ligamento redondo*/
                glTranslated(+34, -32, 10);
                glRotated(-rot_braco_dir, 1, 0, 0);
                glScaled(1.0, 1.0, 1.7);
                glutSolidSphere(10, slices, stacks);
            glPopMatrix();

            glPushMatrix(); /*Hierarquia antebraco*/
                glPushMatrix(); /*Ligamento braco-antebraco*/
                    glTranslated(+47, -75, 10);
                    glScaled(1.0, 1.0, 1.5);
                    glutSolidSphere(8.5, slices, stacks);
                glPopMatrix();

                glTranslated(+46, -75, 10);
                glRotated(15, 0, 0, 1);
                glRotated(rot_ant_braco_dir, 1, 0, 0);
                glRotated(-15, 0, 0, 1);
                glTranslated(-46, +75, -10);

                glPushMatrix(); /*Desenha antebraco*/
                    glTranslated(+46, -75, 10);
                    glRotated(15, 0, 0, 1);
                    glScaled(1.0, 1.0, 1.4);
                    glRotated(90, 1, 0, 0);
                    glutSolidCylinder(8, 25, slices, stacks);
                glPopMatrix();

                glPushMatrix(); /*Hierarquia mao*/

                    //glPushMatrix(); /*Ligamento antebraco e mao*/
                      /*  glColor3d(0, 0, 0);
                        glTranslated(+52, -97, 10);
                        glScaled(1.0, 1.0, 1.0);
                        glRotated(15, 0, 0, 1);
                        glRotated(90, 1, 0, 0);
                        glutSolidCylinder(8, 10, slices, stacks);
                    glPopMatrix();*/

                        glTranslated(+53, -105, 10);
                        glRotated(15, 0, 0, 1);
                        glRotated(rot_mao_dir, 1, 0, 0);
                        glRotated(-15, 0, 0, 1);
                        glTranslated(-53, +105, -10);

                        //Mao
                        glPushMatrix();
                            glColor3d(0, 0, 0);
                            glTranslated(+53, -105, 10);
                            glRotated(15, 0, 0, 1);
                            glScaled(1.0, 1.4, 1.4);
                            glRotated(90, 1, 0, 0);
                            //glutSolidCylinder(8, 10, slices, stacks);
                            glutSolidSphere(10, slices, stacks);
                        glPopMatrix();

                        if (flag_animacao == 1){
                            //Lupa
                            glPushMatrix();
                                glColor3f(0.2, 0.29, 0.37);
                                glTranslated(55, -135, 10);
                                glutSolidTorus(2, 10, slices, nrings);
                            glPopMatrix();

                            glPushMatrix();
                                glColor3f(0.2, 0.29, 0.37);
                                glTranslated(55, -110, 10);
                                //glRotated(-15, 0, 0, 1);
                                glScaled(1.0, 3.5, 0.5);
                                glRotated(90, 1, 0, 0);
                                glutSolidCylinder(4, 5, slices, stacks);
                            glPopMatrix();
                        }

                glPopMatrix(); /*FIM Hierarquia mao*/

            glPopMatrix(); /*FIM Hierarquia antebraco*/

        glPopMatrix(); /*FIM Hierarquia braco direito*/

    glPopMatrix();
}

void desenha_arvore(){

    //Copa da arvore
    glPushMatrix();
        glColor3d(0.0, 0.50, 0.50);
        glTranslated(-1000, 100, 520);
        glScaled(90, 90, 90);
        glutSolidDodecahedron();
    glPopMatrix();

    //Tronco da arvore
    glPushMatrix();
        glColor3d(0.91, 0.59, 0.48);
        glTranslated(-1000, -60, 520);
        glScaled(40, 200, 40);
        glutSolidCube(2);
    glPopMatrix();

}

void desenha_montanha(){

    for (int i = 0; i < 5; i++){
        glPushMatrix();
            glColor3d(0.87, 0.72, 0.53);
            glTranslated(((i-1) * 800), 0, 900);
            glRotated(90, 1, 0, 0);
            glRotated(90, 0, 1, 0);
            glScaled(500, 700 + 20*i, 700);
            glutSolidTetrahedron();
        glPopMatrix();
    }

    for (int i = 0; i < 5; i++){
        glPushMatrix();
            glColor3d(0.87, 0.72, 0.53);
            glTranslated(((i-1) * 800), 250, 1400);
            glRotated(90, 1, 0, 0);
            glRotated(90, 0, 1, 0);
            glScaled(500, 700 + 120*i, 700);
            glutSolidTetrahedron();
        glPopMatrix();
    }

}

void desenha_membros_inferiores(){

    //Quadril
    glColor3d(0.47, 0.29, 0.13);
    glPushMatrix();
        glTranslated(10.0, -100.0, 0.0);
        glScaled(0.8, 1.0, 1.2);
        glRotated(12, 0, 0, 1);
        glRotated(90, 1, 0, 0);
        glutSolidCylinder(18, 45, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-10.0, -100.0, 0.0);
        glScaled(0.8, 1.0, 1.2);
        glRotated(-12, 0, 0, 1);
        glRotated(90, 1, 0, 0);
        glutSolidCylinder(18, 45, slices, stacks);
    glPopMatrix();

    //Cinto
    glColor3d(0, 0, 0);
    glPushMatrix();
        glTranslated(0.0, -100.0, 0.0);
        glScaled(1.2, 1.0, 1.05);
        glRotated(90, 1, 0, 0);
        glutSolidTorus(5, 20, slices, nrings);
    glPopMatrix();

    //Parte amarela do cinto
    glColor3d(1.0, 0.83, 0.53);
    glPushMatrix();
        glTranslated(0.0, -100.0, 22.0);
        glutSolidCube(10);
    glPopMatrix();

    //Gluteos
    //glColor3d(129.0/255.0,59/255.0,9.0/255.0);
    /*
    glColor3d(0.47, 0.29, 0.13);
    glPushMatrix();
        glTranslated(-10.0, -120.0, -10.0);
        glScaled(0.8, 0.7, 0.6);
        glutSolidSphere(20, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(+10.0, -120.0, -10.0);
        glScaled(0.8, 0.7, 0.6);
        glutSolidSphere(20, slices, stacks);
    glPopMatrix();
    */

    //Perna
    glPushMatrix(); //hierarquia da perna
        glTranslated(+15.0, -140.0, 0.0);
        glRotated(rot_perna_dir, 1, 0, 0);
        glTranslated(-15.0, +140.0, 0.0);

        //Coxa esquerda
        glPushMatrix();
            glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
            glTranslated(15.0, -140.0, 0.0);
            glRotated(6, 0, 0, 1);
            glRotated(90, 1, 0, 0);
            glutSolidCylinder(12, 30, slices, stacks);
        glPopMatrix();

        glPushMatrix(); //Hierarquia do joelho
            glPushMatrix(); //Ligamento perna-joelho
                glTranslated(18, -170, 1);
                glRotated(rot_joelho_dir, 1, 0, 0);
                glRotated(6, 0, 0, 1);
                glScaled(1.0, 1.0, 1.1);
                glutSolidSphere(12.01, slices, stacks);
            glPopMatrix();

            glTranslated(17.5, -165.0, 0.0);
            glRotated(+6, 0, 0, 1);
            glRotated(rot_joelho_dir, 1, 0, 0);
            glRotated(-6, 0, 0, 1);
            glTranslated(-17.5, +165.0, 0.0);

            //Bota direita
            glPushMatrix();
                glColor3d(0, 0, 0);
                glTranslated(21.0, -195.0, 0.0);
                glRotated(6, 0, 0, 1);
                glRotated(90, 1, 0, 0);
                glutSolidCylinder(12, 40, slices, stacks);
            glPopMatrix();

            //Joelho direito
            glPushMatrix();
                glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
                glTranslated(17.5, -165.0, 0.0);
                glRotated(6, 0, 0, 1);
                glRotated(90, 1, 0, 0);
                glutSolidCylinder(12, 30, slices, stacks);
            glPopMatrix();

            glPushMatrix(); //Hierarquia dos pes
                glPushMatrix(); //Ligamento batata-pe
                    glColor3d(0, 0, 0);
                    glTranslated(25, -232, 0);
                    glRotated(6, 0, 0, 1);
                    glScaled(1.0, 1.0, 1.1);
                    glutSolidSphere(12.01, slices, stacks);
                glPopMatrix();

                glTranslated(25.0, -235.0, 5.0);
                glRotated(+6, 0, 0, 1);
                glRotated(rot_pe_dir, 1, 0, 0);
                glRotated(-6, 0, 0, 1);
                glTranslated(-25.0, +235.0, -5.0);

                //Pe direito
                glPushMatrix();
                    glColor3d(0, 0, 0);
                    glTranslated(25.0, -235.0, 5.0);
                    glRotated(6, 0, 0, 1);
                    glRotated(90, 1, 0, 0);
                    glScaled(1.0, 1.5, 0.2);
                    glutSolidCylinder(12, 40, slices, stacks);
                glPopMatrix();

            glPopMatrix(); //Fecha hierarquia dos Pes

        glPopMatrix(); //Fecha hierarquia do Joelho

    glPopMatrix(); //Fecha hierarquia da Perna

    //Perna
    glPushMatrix(); //Hierarquia da Perna Esquerda
        glTranslated(-15.0, -140.0, 0.0);
        glRotated(rot_perna_esq, 1, 0, 0);
        glTranslated(+15.0, +140.0, 0.0);

        //Coxa esquerda
        glPushMatrix();
            glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
            glTranslated(-15.0, -140.0, 0.0);
            glRotated(-6, 0, 0, 1);
            glRotated(90, 1, 0, 0);
            glutSolidCylinder(12, 30, slices, stacks);
        glPopMatrix();

        glPushMatrix(); //Hierarquia do joelho
            glPushMatrix(); //Ligamento perna-joelho
                glTranslated(-18, -170, 1);
                glRotated(rot_joelho_esq, 1, 0, 0);
                glRotated(6, 0, 0, 1);
                glScaled(1.0, 1.0, 1.1);
                glutSolidSphere(12.01, slices, stacks);
            glPopMatrix();

            glTranslated(-17.5, -165.0, 0.0);
            glRotated(-6, 0, 0, 1);
            glRotated(rot_joelho_esq, 1, 0, 0);
            glRotated(+6, 0, 0, 1);
            glTranslated(+17.5, +165.0, 0.0);

            //Bota esquerda
            glPushMatrix();
                glColor3d(0, 0, 0);
                glTranslated(-21.0, -195.0, 0.0);
                glRotated(-6, 0, 0, 1);
                glRotated(90, 1, 0, 0);
                glutSolidCylinder(12, 40, slices, stacks);
            glPopMatrix();

            //Joelho esquerdo
            glPushMatrix();
                glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
                glTranslated(-17.5, -165.0, 0.0);
                glRotated(-6, 0, 0, 1);
                glRotated(90, 1, 0, 0);
                glutSolidCylinder(12, 30, slices, stacks);
            glPopMatrix();

            glPushMatrix(); //Hierarquia dos pes
                glPushMatrix(); //Ligamento batata-pe
                    glColor3d(0, 0, 0);
                    glTranslated(-25, -232, 0);
                    glRotated(6, 0, 0, 1);
                    glScaled(1.0, 1.0, 1.1);
                    glutSolidSphere(12.01, slices, stacks);
                glPopMatrix();

                glTranslated(25.0, -235.0, 5.0);
                glRotated(-6, 0, 0, 1);
                glRotated(rot_pe_esq, 1, 0, 0);
                glRotated(6, 0, 0, 1);
                glTranslated(-25.0, +235.0, -5.0);

                //Pe direito
                glPushMatrix();
                    glColor3d(0, 0, 0);
                    glTranslated(-25.0, -235.0, 5.0);
                    glRotated(-6, 0, 0, 1);
                    glRotated(90, 1, 0, 0);
                    glScaled(1.0, 1.5, 0.2);
                    glutSolidCylinder(12, 40, slices, stacks);
                glPopMatrix();

            glPopMatrix(); //Fecha hierarquia dos pes

        glPopMatrix(); //Fecha hierarquia do joelho

    glPopMatrix(); //Fecha hierarquia da perna

}

/*Desenha o templo*/
void desenha_templo(){

    glColor3d(0.25, 0.27, 0.35);
    //Porta do templo
    glPushMatrix();
        glColor3d(0.70, 0.22, 0.22);
        glTranslated(1050, -50, 0);
        glScaled(1.0, 1.7, 1.0);
        glutSolidCube(200);
    glPopMatrix();
    /*
    //Borda da porta do templo
    glPushMatrix();
        glColor3f(0.95, 0.61, 0.07);
        glTranslated(1080, -50, 0);
        glScaled(1.0, 1.7, 1.2);
        glutSolidCube(240);
    glPopMatrix();
    */
     //Templo grande
    glPushMatrix();
        //glColor3d(0.8117, 0.6235, 0.3019);
        glColor3d(255.0/255.0,222.0/255.0,173.0/255.0);
        glTranslated(1250, 5, 0);
        glScaled(1.0, 0.9, 2.0);
        glutSolidCube(500);
    glPopMatrix();

    //2a parte superior
    glPushMatrix();
        glColor3d(255.0/255.0, 235.0/255.0, 180.0/255.0);
        glTranslated(1250, 250, 0);
        glScaled(1.5, 0.2, 3.0);
        glutSolidCube(400);
    glPopMatrix();

    glColor3d(0.70, 0.22, 0.22);

    //3a parte superior
    glPushMatrix();
        glTranslated(1250, 320, 0);
        glScaled(1.3, 0.2, 3.0);
        glutSolidCube(400);
    glPopMatrix();

    //4a parte superior
    glPushMatrix();
        glTranslated(1300, 400, 0);
        glScaled(0.9, 0.2, 2.5);
        glutSolidCube(400);
    glPopMatrix();

    //5a parte superior
    glPushMatrix();
        glTranslated(1350, 450, 0);
        glScaled(0.7, 0.2, 2.0);
        glutSolidCube(400);
    glPopMatrix();

    /*
    //Triangulo do templo
    glPushMatrix();
        glColor3f(0.83, 0.33, 0.0);
        glTranslated(1020, 350, 0);
        glScaled(100, 300, 500);
        glutSolidTetrahedron();
    glPopMatrix();

    //Placa do templo
    glPushMatrix();
        glColor3d(0.8117, 0.6235, 0.3019);
        glTranslated(1000, 350, 0);
        glScaled(100, 100, 600);
        glutSolidCube(1);
    glPopMatrix();
    */

}

void desenha_fogueira(){

    //Desenha a Fogueira
    glPushMatrix();
        glTranslated(-1900, 0, +200);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glPushMatrix();
            glTranslated(1800, -200, -725);
            glRotated(-90, 1, 0, 0);
            glutSolidCone(50, 90, slices, stacks);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glColor3d(0.47, 0.29, 0.13);
    //Lenha da Fogueira
    glPushMatrix();
        glTranslated(1880, -200, -650);
        glRotated(45, 0, 1, 0);
        glRotated(90, 1, 0, 0);
        glScaled(0.3, 8.0, 0.3);
        glRotated(90, 1, 0, 0);
        glutSolidCylinder(20, 26, slices, stacks);
    glPopMatrix();

    //Lenha da Fogueira
    glPushMatrix();
        glTranslated(1730, -200, -650);
        glRotated(-45, 0, 1, 0);
        glRotated(90, 1, 0, 0);
        glScaled(0.3, 8.0, 0.3);
        glRotated(90, 1, 0, 0);
        glutSolidCylinder(20, 26, slices, stacks);
    glPopMatrix();

    glPopMatrix();

}

void desenha_ruina(){

    glColor3d(0.87, 0.72, 0.53);

    glPushMatrix();
        glTranslated(-500, -200, 300);
        glRotated(45, 0, 1, 0);
        glTranslated(+500, +200, -300);

    //Ponta
    glPushMatrix();
        glTranslated(-500, -200, 300);
        glScaled(2, 1, 1);
        glutSolidCube(70);
    glPopMatrix();

    //Meio
    glPushMatrix();
        glTranslated(-500, -200, 100);
        glScaled(1, 1, 6);
        glutSolidCube(70);
    glPopMatrix();

    //Ponta
    glPushMatrix();
        glTranslated(-500, -200, -100);
        glScaled(2, 1, 1);
        glutSolidCube(70);
    glPopMatrix();

    glPopMatrix();

    glPushMatrix();
        glTranslated(-700, -160, -100);
        glutSolidSphere(70, slices, stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-730, -160, -350);
        glutSolidSphere(60, slices, stacks);
    glPopMatrix();


    glPushMatrix();
        glTranslated(-850, -160, -250);
        glutSolidSphere(50, slices, stacks);
    glPopMatrix();

}

void desenha_estatua(){
    glColor3d(0.87, 0.72, 0.53);

    glPushMatrix();
        glTranslated(1980, 0, 200);

        //Base
        glPushMatrix();
            glTranslated(0, -180, 0);
            glScaled(1, 1, 2);
            glutSolidCube(60);
        glPopMatrix();

        //Corpo
        glPushMatrix();
            glTranslated(0, -120, 0);
            glScaled(1, 1.5, 0.8);
            glutSolidSphere(50, slices, stacks);
        glPopMatrix();

        //Cabeca
        glPushMatrix();
            glTranslated(0, -30, 0);
            glutSolidSphere(30, slices, stacks);
        glPopMatrix();
    glPopMatrix();

}

void desenha_montanha_spline(){

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 3, &ctrlpoints1[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glColor3d(0.76, 0.45, 0.25);

    for (int i = 0; i < 100; i++){
    glPushMatrix();
        glTranslated(0, 100, i * 2);
        glScaled(65, 65, 65);
        glBegin(GL_POLYGON);
        for (int i = 0; i <= 100; i++)
            glEvalCoord1f((GLfloat) i/100.0);
        glEnd();
    glPopMatrix();
    }

    for (int i = 0; i < 100; i++){
        glPushMatrix();
            glTranslated(320, +200, i * 2);
            glScaled(90, 90, 90);
            glBegin(GL_POLYGON);
            for (int i = 0; i <= 100; i++)
                glEvalCoord1f((GLfloat) i/100.0);
            glEnd();
        glPopMatrix();
    }
    /*
    glPointSize(10);
    glBegin(GL_POINTS);
    for (int i = 0; i < 6; i++){
        glColor3f((i+1)*0.15, (i+1)*0.04, (i+1)*0.06);
        glVertex2f(ctrlpoints1[i][0], ctrlpoints1[i][1]);
    }
    glEnd();
    */

    glFlush();

}

/*Funcao principal para o desenhos do caminhao e as toras*/
void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*Desenha cena*/
    glPushMatrix();

        glRotated(ang_x, 1, 0, 0);
        glRotated(ang_y, 0, 1, 0);
        glRotated(ang_z, 0, 0, 1);
        glScaled(escala, escala, escala);

        if (flag_cena == 1){
            glPushMatrix();
                glTranslated(2700, 400, 0);
                glScaled(3, 4, 3);
                glRotated(90, 0, 1, 0);
                desenha_montanha_spline();
            glPopMatrix();
        }

        if (flag_cena == 1){
            desenha_estatua();
        }

        if (flag_cena == 1){
            glPushMatrix();
                glTranslated(0, 0, -600);
                desenha_estatua();
            glPopMatrix();
        }

        if (flag_cena == 1){
            desenha_ruina();
        }

        if (flag_cena == 1){
            glPushMatrix();
                desenha_fogueira();
            glPopMatrix();
        }

        /*Desenha o objeto atual e os objetos da fila na fila*/
        if (filaAux != NULL){
            /*Desenha o objeto atual sendo cacado*/
            tipoDado *aux = filaAux->primeiro;
            //andando = 1;
            glPushMatrix();
                glTranslated(aux->x - 40, -10 + y_lanca, aux->y - 50);
                glRotated(rot_lanca, 1, 0, 0);

                glPushMatrix();
                    glColor3d(1, 1, 1);
                    glTranslated(0, -190, 0);
                    glRotated(90, 1, 0, 0);
                    glutSolidCone(15, 30, slices, stacks);
                glPopMatrix();

                glColor3f(0.9, 0.49, 0.13);
                glScaled(0.3, 8.0, 0.3);
                glRotated(90, 1, 0, 0);
                glutSolidCylinder(20, 26, slices, stacks);
                //glutSolidCube(25);
            glPopMatrix();

            /*Desenha os objetos aguardando na fila*/
            aux = aux->prox;
            while (aux != NULL){
                glPushMatrix();
                    glTranslated(aux->x - 40, -10, aux->y - 50);

                    glPushMatrix();
                        glColor3d(1, 1, 1);
                        glTranslated(0, -190, 0);
                        glRotated(90, 1, 0, 0);
                        glutSolidCone(15, 30, slices, stacks);
                    glPopMatrix();

                    glColor3f(0.9, 0.49, 0.13);
                    glScaled(0.3, 8.0, 0.3);
                    glRotated(90, 1, 0, 0);
                    glutSolidCylinder(20, 26, slices, stacks);
                glPopMatrix();

                aux = aux->prox;
            }
        }

        if (flag_cena == 1){
            glPushMatrix();
                glTranslated(110, 0, 0);
                glTranslated(1000, 0, -100);
                desenha_templo();
            glPopMatrix();
        }

        glPushMatrix(); //Inicia desenho do plano
            glColor3d(0.36, 0.36, 0.36);
            //glColor3d(0.0, 0.50, 0.50);
            glTranslated(100, -230, -200);
            glScaled(26, 0.1, 15);
            glutSolidCube(200);
        glPopMatrix(); //Finaliza desenho do plano

        //Desenha Arvores
        glPushMatrix();
            if (flag_cena == 1){
            glPushMatrix();
                for (int i = 0; i < 10; i++){
                    glPushMatrix();
                        glTranslated(500*(i-2), 0, +300);
                        desenha_arvore();
                    glPopMatrix();
                }
            glPopMatrix();

            glPushMatrix();
                for (int i = 0; i < 10; i++){
                    glPushMatrix();
                        glTranslated(180*i, 0, -1300);
                        //desenha_arvore();
                    glPopMatrix();
                }
            glPopMatrix();
            }
        glPopMatrix();

        /*
        if (flag_cena == 1){
            //Desenha montanha
            glPushMatrix();
                glTranslated(0, 0, 80);
                desenha_montanha();
            glPopMatrix();
        }
        */

        glPushMatrix(); /*Desenha, escala, move e rotaciona Lara*/
            glTranslated(x_pos, -20.0, y_pos);

            glScaled(0.8, 0.8, 0.8);
            glRotated(rotacao, 0, 1, 0);
            glRotated(90, 0, 1, 0);

            desenha_membros_inferiores();

            glPushMatrix();
                glRotated(rot_tronco, 0, 1, 0);
                desenha_bracos();
                desenha_tronco();
                glPushMatrix();
                    glTranslated(0, 2, 0);
                    if (flag_animacao == 1)
                        glRotated(15, 1, 0, 0);
                    glRotated(rot_cabeca, 0, 1, 0);
                    glScaled(0.9, 1.0, 0.9);
                    desenha_cabeca();
                glPopMatrix();
            glPopMatrix();

        glPopMatrix(); /*Fim desenha, escala, move e rotaciona Lara*/

    glPopMatrix();

    glutSwapBuffers();
}

/*Funcao para leitura das teclas para rotacao de partes do corpo da Lara e escala*/
static void funcoes_teclas(unsigned char key, int x, int y){
    switch (key){
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'x':
            ang_x += 10;
            break;
        case 'y':
            ang_y += 10;
            break;
        case 'z':
            ang_z += 10;
            break;
        case 'X':
            ang_x -= 10;
            break;
        case 'Y':
            ang_y -= 10;
            break;
        case 'Z':
            ang_z -= 10;
            break;
        case 's':
            if (escala > 0.02)
                escala -= 0.01;
            break;
        case 'S':
            if (escala < 5)
                escala += 0.01;
            break;
        case 'R':
            rotacao += 10;
            break;
        case 'r':
            rotacao -= 10;
            break;
        case 'T':
            if (rot_tronco > -20)
                rot_tronco -= 5.0;
            break;
        case 't':
            if (rot_tronco < 20)
                rot_tronco += 5.0;
            break;
        case 'G':
		       angle = angle - 5.0;
		    break;
		case 'g':
            angle = angle + 5.0;
		    break;
        case 'a':
            flag_animacao = 0;
            s = 0;
            rotacao = 0;
            andando = 0;
            x_pos = 0.0, y_pos = 0.0;
            rot_braco_esq = rot_braco_dir = rot_ant_braco_dir = rot_ant_braco_esq = rot_perna_dir = rot_perna_esq = 0.0;
            rot_joelho_dir = rot_joelho_esq = rot_cabeca = rot_mao_dir = rot_mao_esq = 0.0;
            break;
        case 'A':
            flag_animacao = 1;
            rot_cabeca = 40;
            andando = 1;
            break;
        case 'p':
            pan_x += 2;
            break;
        case 'P':
            pan_x -= 2;
            break;
        case 'o':
            pan_y += 2;
            break;
        case 'O':
            pan_y -= 2;
            break;
        case 'l':
            pan_z += 2;
            break;
        case 'L':
            pan_z -= 2;
        default:
            break;
    }

    inicializacao();
    glutPostRedisplay();

}

/*Faz o reshape da cena, se o tamanho da janela foi alterado*/
void atualiza_janela(GLsizei w, GLsizei h){

    /* Para previnir uma divisão por zero */
	if (h == 0)
		h = 1;

    glViewport(0, 0, w, h);

	/* Calcula a correção de aspecto */
	fAspect = (GLfloat)w/(GLfloat)h;
	//largura_tela = w/2.0;
	//altura_tela = h/2.0;

    inicializacao();

}

/*Iluminacao da cena*/
void ilumina(){
    GLfloat luz_ambiente[]  = {0.5, 0.5, 0.5, 1.0};
    GLfloat luz_difusa[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat luz_especular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat posicao_luz[] = {0.0, 2.0, 1.0, 0.0};

    GLfloat mat_ambiente[] = {0.7, 0.7, 0.7, 1.0};
    GLfloat mat_difuso[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat mat_especular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat brilho[] = {100.0f};

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, luz_ambiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_difusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luz_especular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambiente);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_difuso);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_especular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, brilho);
}

/*Faz o carregamento do arquivo bmp*/
void get_bitmap_image_data(char *pFileName, BMPImage *pImage ){
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
	int i;

    if((pFile = fopen(pFileName, "rb")) == NULL)
		printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

    fseek(pFile, 18, SEEK_CUR);

    if((i = fread(&pImage->width, 4, 1, pFile)) != 1)
		printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ", pFileName);

    if((i = fread(&pImage->height, 4, 1, pFile)) != 1)
		printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ", pFileName);

    if((fread(&nNumPlanes, 2, 1, pFile)) != 1)
		printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);

    if(nNumPlanes != 1)
		printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);

    if((i = fread(&nNumBPP, 2, 1, pFile)) != 1)
		printf( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ", pFileName);

    if(nNumBPP != 24)
		printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);

    fseek(pFile, 24, SEEK_CUR);

    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );

    if((i = fread(pImage->data, nTotalImagesize, 1, pFile)) != 1)
		printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);

	char charTemp;
    for(i = 0; i < nTotalImagesize; i += 3){
		charTemp = pImage->data[i];
		pImage->data[i] = pImage->data[i+2];
		pImage->data[i+2] = charTemp;
    }
}

/*Carrega a Textura*/
void carrega_textura(){
    BMPImage textura;
    char nome[15] = "fire.bmp";
    get_bitmap_image_data(nome, &textura);

    /*Definicao dos parametros da textura*/
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textura.width, textura.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura.data);

    /*Define modo de preenchimento se textura for menor ou maior*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    //Mapeamento esferico
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

/*Faz a leitura das coordenadas atuais do mouse*/
void interacao_mouse(int botao, int estado, int x, int y){
    /* Checa se algum botao foi presionado*/
    if (botao == GLUT_LEFT_BUTTON && estado == GLUT_DOWN){
        /*Checa os limites da tela*/
        x = rand() % 2000;
        y = rand() % 500;

        /*Transforma as coordenados do mouse*/
        x_mouse_novo = x - largura_tela/2;
        y_mouse_novo = - y + altura_tela/2;

        if (x_mouse_novo < 0)
            x_mouse_novo = -x_mouse_novo;

        if (x_mouse_novo > 3000)
            x_mouse_novo = 3000;

        if (x_mouse_novo < 400)
            x_mouse_novo = 100;

        /*Insere as coordenadas na fila*/
        insereFinal(x_mouse_novo, y_mouse_novo);
    }
}

/*Funcao que calcula a posicao do caminhao e etc*/
void move_lara(){
    static double passo = 0.0;

        if (estado == 0){
            //menu_opcoes(14);
            esq = 0;
            if (filaAux != NULL){
                x_mouse = filaAux->primeiro->x;
                y_mouse = filaAux->primeiro->y;
                if (y_pos < y_mouse) { //virar a esquerda
                    esq = 1;
                    andando = 1;
                    if (rotacao > -90)
                        rotacao -= velocidade_rotacao;
                    else {
                        andando = 1;
                        estado = 1;
                    }
                }
                else { //virar a direita
                    andando = 1;
                    if (rotacao < 90)
                        rotacao += velocidade_rotacao;
                    else {
                        andando = 1;
                        estado = 1;
                    }
                }
            }
            else
                estado = 0;
        }

        if (estado == 1){
            andando = 1;
            if (esq == 1 && y_pos < y_mouse){
                y_pos += velocidade_lara;
            }
            else if (esq == 0 && y_pos > y_mouse){
                y_pos -= velocidade_lara;
            }

            if (esq == 1 && y_pos >= y_mouse){
                andando = 1;
                if (rotacao < 0)
                    rotacao += velocidade_rotacao;
                else {
                    estado = 2;
                    andando = 1;
                }
            }

            if (esq == 0 && y_pos <= y_mouse){
                andando = 1;
                if (rotacao > 0)
                    rotacao -= velocidade_rotacao;
                else {
                    andando = 1;
                    estado = 2;
                }
            }
        }

        if (estado == 2){
            if (x_pos <= x_mouse){
                x_pos += velocidade_lara;
            }
            else {
                if (esq == 1)
                    if (rotacao < 90)
                        rotacao += velocidade_rotacao;
                    else
                        estado = 21;
                else
                    if (rotacao < 90)
                        rotacao += velocidade_rotacao;
                    else
                        estado = 21;

            }
            cont++;
        }

        if (estado == 21){ //estica a mao
            andando = 0;
            parou_tudo = 1;
            rot_braco_dir = 0;
            rot_braco_dir = -80;
            estado = 22;
        }

        if (estado == 22){
            if (rot_lanca < 90) {
                rot_braco_dir -= 0.15;
                rot_lanca += 1.0;
                filaAux->primeiro->y += 1;
                y_lanca -= 0.3;
            }
            else
                estado = 23;

        }

        if (estado == 23){
            if (rot_braco_dir > -120){
                rot_braco_dir -= 2;
                rot_lanca += 2;
                y_lanca -= 0.5;
            }
            else
                estado = 24;
        }

        if (estado == 24){
            if (rot_braco_dir < -90){
                rot_braco_dir += 1;
                rot_lanca -= 1;
                y_lanca += 0.25;
            }
            else
                estado = 25;
        }

        if (estado == 25){
            if (filaAux->primeiro->y > -1650)
                filaAux->primeiro->y -= 30;
            else
                estado = 26;
        }

        if (estado == 26){
            if (rot_braco_dir < 0.0)
                rot_braco_dir += velocidade_rotacao;
            else
                estado = 27;
        }

        if (estado == 27){
            rot_lanca = 0;
            y_lanca = 0;
            if (esq == 1) {
                if (rotacao > -180)
                    rotacao -= velocidade_rotacao;
                else
                    estado = 3;
            }
            else {
                if (rotacao < 180)
                    rotacao += velocidade_rotacao;
                else
                    estado = 3;
            }
        }

        if (estado == 3){
            andando = 1;
            if (x_pos > 0.0){
                x_pos -= velocidade_lara;
            }
            else {
                if (esq == 1){
                    if (rotacao > -270)
                        rotacao -= velocidade_rotacao;
                    else
                        estado = 4;
                }
                else {
                    if (rotacao < 270)
                        rotacao += velocidade_rotacao;
                    else
                        estado = 4;
                }
            }
        }

        if (estado == 4){
            if (esq == 1 && y_pos >= 0.0){
                y_pos -= velocidade_lara;
            }
            else if (y_pos <= 0.0) {
                y_pos += velocidade_lara;
            }

            if (esq == 1 && y_pos < 0.0){
                if (rotacao > -360)
                    rotacao -= velocidade_rotacao;
                else {
                    estado = 5;
                    andando = 0;
                }
            }
            if (esq == 0 && y_pos > 0.0) {
                if (rotacao < 360)
                    rotacao += velocidade_rotacao;
                else {
                    estado = 5;
                    andando = 0;
                }
            }
        }

        if (estado == 5){
            x_pos = 0.0;
            y_pos = 0.0;
            rotacao = 0;
            estado = 0;

            if (filaAux->primeiro->prox != NULL){
                filaAux->primeiro = filaAux->primeiro->prox;
                estado = 0;
                andando = 1;
            }
            else {
                //menu_opcoes(14);
                parou_tudo = 0;
                x_mouse = y_mouse = 0;
                estado = 0;
                filaAux = NULL;
            }
        }

    if (andando == 1){
        rot_perna_dir = 20.0 * sin(passo * (3.14/180.0)); // Movimenta perna direita
        rot_braco_esq = 30.0 * sin(passo * (3.14/180.0)); // Movimenta ombro direito
        rot_ant_braco_esq = -10.0; // cotovelo direito
        rot_perna_esq = -rot_perna_dir;
        rot_braco_dir = -rot_braco_esq;
        rot_ant_braco_dir = -rot_ant_braco_esq;
        rot_joelho_esq = -20.0 * sin(passo * (3.14/180.0)); // Movimenta joelho esquerdo
        if(rot_joelho_esq <= 0.0) rot_joelho_esq = 0.0;

        rot_joelho_dir = 20.0 * sin(passo * (3.14/180.0)); // Movimenta joelho esquerdo
        if(rot_joelho_dir <= 0.0) rot_joelho_dir = 0.0;

        passo = fmod(passo + 8.0, 360.0); // incrementa o passo na velocidade 8
    }
    else if (!parou_tudo){
        if(passo == 0.0) return;

        rot_perna_dir = 20.0 * sin(passo * (3.14/180.0));
        rot_braco_esq = 30.0 * sin(passo * (3.14/180.0));
        rot_ant_braco_esq = -10.0;
        rot_perna_esq = -rot_perna_dir;
        rot_braco_dir = -rot_braco_esq;
        rot_ant_braco_dir = -rot_ant_braco_esq;

        rot_joelho_esq = -20.0 * sin(passo * (3.14/180.0));
        if(rot_joelho_esq <= 0.0) rot_joelho_esq = 0.0;

        rot_joelho_dir = 20.0 * sin(passo * (3.14/180.0));
        if(rot_joelho_dir <= 0.0) rot_joelho_dir = 0.0;

        if (passo < 180.0){
                if ((passo + 8) > 180.0) passo = 0.0;
                else passo += 8;
            }
            else{
                if ((passo + 8) > 360.0) passo = 0.0;
                else passo += 8;
            }
        return;
    }

}

/*Funcao responsavel por animar a cena*/
void atualiza_cena(int t){

    if (flag_animacao == 1){
        animacao();
    }
    else {
        move_lara();
    }

    glutPostRedisplay();
    glutTimerFunc(25, atualiza_cena, 1);

}

/*Verifica qual a posicao deve rodar a parte escolhida pelo usuario*/
void verifica_seta(int key, int x, int y){

    if(key == GLUT_KEY_LEFT) {
       soma_rot = -10;
    }

    if(key == GLUT_KEY_RIGHT) {
        soma_rot = +10;
    }

    faz_opcao_menu();
    glutPostRedisplay();

}

/*Realiza a opcao escolhida pelo usuario*/
void faz_opcao_menu(){

    if (flag_braco_esq == 1){
        if (soma_rot > 0 && rot_braco_esq < 40)
            rot_braco_esq += soma_rot;
        if (soma_rot < 0 && rot_braco_esq > -180)
            rot_braco_esq += soma_rot;
    }
    if (flag_braco_dir == 1){
        if (soma_rot > 0 && rot_braco_dir < 40)
            rot_braco_dir += soma_rot;
        if (soma_rot < 0 && rot_braco_dir > -180)
            rot_braco_dir += soma_rot;
    }

    if (flag_antebraco_esq == 1){
        if (soma_rot > 0 && rot_ant_braco_esq < 20)
            rot_ant_braco_esq += soma_rot;
        if (soma_rot < 0 && rot_ant_braco_esq > -140)
            rot_ant_braco_esq += soma_rot;
    }

    if (flag_antebraco_dir == 1){
        if (soma_rot > 0 && rot_ant_braco_dir < 20)
            rot_ant_braco_dir += soma_rot;
        if (soma_rot < 0 && rot_ant_braco_dir > -140)
            rot_ant_braco_dir += soma_rot;
    }

    if (flag_perna_esq == 1){
        if (soma_rot > 0 && rot_perna_esq < 40)
            rot_perna_esq += soma_rot;
        if (soma_rot < 0 && rot_perna_esq > -50)
            rot_perna_esq += soma_rot;
    }
    if (flag_perna_dir == 1){
        if (soma_rot > 0 && rot_perna_dir < 40)
            rot_perna_dir += soma_rot;
        if (soma_rot < 0 && rot_perna_dir > -50)
            rot_perna_dir += soma_rot;
    }

    if (flag_joelho_esq == 1){
        if (soma_rot > 0 && rot_joelho_esq < 40)
            rot_joelho_esq += soma_rot;
        if (soma_rot < 0 && rot_joelho_esq > -40)
            rot_joelho_esq += soma_rot;
    }
    if (flag_joelho_dir == 1){
        if (soma_rot > 0 && rot_joelho_dir < 40)
            rot_joelho_dir += soma_rot;
        if (soma_rot < 0 && rot_joelho_dir > -40)
            rot_joelho_dir += soma_rot;
    }

    if (flag_cabeca == 1){
        if (soma_rot > 0 && rot_cabeca < 40)
            rot_cabeca += soma_rot;
        if (soma_rot < 0 && rot_cabeca > -40)
            rot_cabeca += soma_rot;
    }

    if (flag_mao_esq == 1){
        if (soma_rot > 0 && rot_mao_esq < 90)
            rot_mao_esq += soma_rot;
        if (soma_rot < 0 && rot_mao_esq > -90)
            rot_mao_esq += soma_rot;
    }
    if (flag_mao_dir == 1){
        if (soma_rot > 0 && rot_mao_dir < 90)
            rot_mao_dir += soma_rot;
        if (soma_rot < 0 && rot_mao_dir > -90)
            rot_mao_dir += soma_rot;
    }

    glutPostRedisplay();

}

/*Seta as flags de acordo com a opcao escolhida pelo usuario*/
void menu_opcoes(GLint opcao) {
    flag_antebraco_dir = flag_antebraco_esq = flag_perna_dir = flag_perna_esq = flag_joelho_dir = flag_joelho_esq = 0;
    flag_braco_esq = flag_braco_dir = flag_cabeca = flag_animacao = flag_mao_dir = flag_mao_esq = 0;

    switch(opcao){
    case 1:
        exit(0);
        break;
    case 2:
        flag_braco_esq = 1;
        break;
    case 3:
        flag_braco_dir = 1;
        break;
    case 4:
        flag_antebraco_esq = 1;
        break;
     case 5:
        flag_antebraco_dir = 1;
        break;
    case 6:
        flag_perna_esq = 1;
        break;
    case 7:
        flag_perna_dir = 1;
        break;
    case 8:
        flag_joelho_esq = 1;
        break;
    case 9:
        flag_joelho_dir = 1;
        break;
    case 10:
        flag_cabeca = 1;
        break;
    case 11:
        rot_cabeca = 40;
        flag_animacao = 1;
        andando = 1;
        break;
    case 12:
        flag_cena = 0;
        break;
    case 13:
        flag_cena = 1;
        break;
    case 14:
        rot_braco_esq = rot_braco_dir = rot_ant_braco_dir = rot_ant_braco_esq = rot_perna_dir = rot_perna_esq = 0.0;
        rot_joelho_dir = rot_joelho_esq = rot_cabeca = rot_mao_dir = rot_mao_esq = 0.0;
        break;
    case 15:
        flag_animacao = 0;
        s = 0;
        rotacao = 0;
        andando = 0;
        x_pos = 0.0, y_pos = 0.0;
        rot_braco_esq = rot_braco_dir = rot_ant_braco_dir = rot_ant_braco_esq = rot_perna_dir = rot_perna_esq = 0.0;
        rot_joelho_dir = rot_joelho_esq = rot_cabeca = rot_mao_dir = rot_mao_esq = 0.0;
        break;
    case 16:
        flag_visao = 1;
        break;
    case 17:
        flag_visao = 2;
        break;
    case 18:
        flag_mao_esq = 1;
        break;
    case 19:
        flag_mao_dir = 1;
        break;
    default:
        break;
    }
}

/*Faz a animcao automatica da Lara*/
void animacao(){
    static double passo = 0.0;

    if (s == 0){
        andando = 1;
        rot_ant_braco_dir = -120;
        rot_ant_braco_esq = -100;
        s = 1;
    }

    if (s == 1){
        if (x_pos < 1900){
            x_pos += velocidade_lara;
        }
        else {
            s = 2;
            andando = 0;
        }
    }

    if (s == 2){
        andando = 1;
        if (rotacao > -180)
            rotacao -= velocidade_rotacao;
        else
            s = 3;
    }

    if (s == 3){
        andando = 1;
        if (x_pos > 0)
            x_pos -= velocidade_lara;
        else {
            s = 4;
            andando = 0;
        }
    }

    if (s == 4){
        andando = 1;
        if (rotacao <= 0)
            rotacao += velocidade_lara;
        else
            s = 0;
    }

    if (andando == 1){
        rot_cabeca = 50.0 * sin(passo * (3.14/180.0));
        rot_perna_dir = 20.0 * sin(passo * (3.14/180.0));
        rot_perna_esq = -rot_perna_dir;

        rot_joelho_esq = -20.0 * sin(passo * (3.14/180.0));
        if(rot_joelho_esq <= 0.0) rot_joelho_esq = 0.0;

        rot_joelho_dir = 20.0 * sin(passo * (3.14/180.0));
        if(rot_joelho_dir <= 0.0) rot_joelho_dir = 0.0;

        passo = fmod(passo + 8.0, 360.0);
    }
    else if (!parou_tudo){
        if(passo == 0.0) return;
        rot_cabeca = 20.0 * sin(passo * (3.14/180.0));
        rot_perna_dir = 20.0 * sin(passo * (3.14/180.0));
        rot_perna_esq = -rot_perna_dir;

        rot_joelho_esq = -20.0 * sin(passo * (3.14/180.0));
        if(rot_joelho_esq <= 0.0) rot_joelho_esq = 0.0;

        rot_joelho_dir = 20.0 * sin(passo * (3.14/180.0));
        if(rot_joelho_dir <= 0.0) rot_joelho_dir = 0.0;

        if (passo < 180.0){
                if ((passo + 8) > 180.0) passo = 0.0;
                else passo += 8;
            }
            else{
                if ((passo + 8) > 360.0) passo = 0.0;
                else passo += 8;
            }
        return;
    }

}

/*Programa principal*/
int main(int argc, char** argv){
    SetCurrentDirectory("C:\\Users\\willi\\Downloads\\Unifesp\\2018.2\\Computação Gráfica\\Projeto Final\\Projeto Final CG - Willian 112269\\Código - Lara Croft World");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH  | GLUT_RGBA);
	glutInitWindowSize(largura_tela, altura_tela);
	glutCreateWindow("Lara Croft World");
    glutDisplayFunc(display);
    glutReshapeFunc(atualiza_janela);
    glutKeyboardFunc(funcoes_teclas);
    glutMouseFunc(interacao_mouse);
    glutTimerFunc(25, atualiza_cena, 0);
    glClearColor(0.031, 0.22, 0.32, 1); //noite
    //glClearColor(0.0, 0.604, 0.95, 1); //dia

    inicializacao();

    ilumina();

    carrega_textura();

    glutCreateMenu(menu_opcoes);
        glutAddMenuEntry("Finalizar", 1);
        glutAddMenuEntry("Rotacionar Braco Esquerdo", 2);
        glutAddMenuEntry("Rotacionar Braco Direito", 3);
        glutAddMenuEntry("Rotacionar Antebraco Esquerdo", 4);
        glutAddMenuEntry("Rotacionar Antebraco Direito", 5);
        glutAddMenuEntry("Rotacionar Mao Esquerda", 18);
        glutAddMenuEntry("Rotacionar Mao Direita", 19);
        glutAddMenuEntry("Rotacionar Perna Esquerda", 6);
        glutAddMenuEntry("Rotacionar Perna Direita", 7);
        glutAddMenuEntry("Rotacionar Joelho Esquerdo", 8);
        glutAddMenuEntry("Rotacionar Joelho Direito", 9);
        glutAddMenuEntry("Rotacionar Cabeca", 10);
        glutAddMenuEntry("Iniciar Animacao Automatica", 11);
        glutAddMenuEntry("Terminar Animacao Automatica", 15);
        glutAddMenuEntry("Carregar Apenas Modelo", 12);
        glutAddMenuEntry("Carregar Cena Completa", 13);
        glutAddMenuEntry("Reiniciar Movimentos", 14);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutSpecialFunc(verifica_seta);

    faz_opcao_menu();

    glutMainLoop();

    return 0;
}

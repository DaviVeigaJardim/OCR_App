# Aplicativo OCR com Qt, OpenCV e Tesseract

Este projeto é um aplicativo de Reconhecimento Óptico de Caracteres (OCR) que permite aos usuários carregar imagens contendo texto e extrair esse texto de forma eficiente. Ele foi desenvolvido utilizando as bibliotecas Qt para interface gráfica, OpenCV para processamento de imagens e Tesseract OCR para a extração de texto.

## Funcionalidades

* **Carregar Imagem:** Permite ao usuário selecionar e carregar imagens nos formatos PNG, JPG e JPEG.
* **Exibição de Imagem:** Exibe a imagem original carregada na interface.
* **Pré-processamento de Imagem:** Oferece opções de pré-processamento como escala de cinza e binarização, com checkboxes para ativar/desativar cada opção.
* **Exibição de Imagem Pré-processada:** Exibe a imagem após o pré-processamento, permitindo ao usuário visualizar as transformações aplicadas.
* **Executar OCR:** Extrai o texto da imagem (após o pré-processamento, se ativado) e exibe o texto extraído em uma área de texto.
* **Interface de Rolagem:** Utiliza `QScrollArea` para permitir a visualização de imagens grandes sem distorcer a interface.

## Tecnologias Utilizadas

* **Qt:** Framework de desenvolvimento de aplicativos multiplataforma utilizado para criar a interface gráfica do usuário (GUI).
* **OpenCV:** Biblioteca de visão computacional utilizada para processamento e manipulação de imagens.
* **Tesseract OCR:** Motor de OCR de código aberto utilizado para extrair texto das imagens.

## Configuração e Compilação

Para compilar e executar o projeto, siga estas etapas:

1.  **Instalação das Dependências:**
    * Certifique-se de ter as bibliotecas Qt, OpenCV e Tesseract instaladas em seu sistema.
    * Instale os arquivos de dados de idioma do Tesseract (por exemplo, `por.traineddata` para português).
2.  **Compilação:**
    * Utilize um compilador C++ compatível com Qt (por exemplo, g++) para compilar os arquivos `OcrApp.cpp` e `OcrApp.h`.
    * Inclua os caminhos dos cabeçalhos e bibliotecas Qt, OpenCV e Tesseract nos comandos de compilação.
3.  **Execução:**
    * Execute o arquivo executável gerado.

## Uso do Aplicativo

1.  **Carregar Imagem:** Clique no botão "Carregar Imagem" e selecione um arquivo de imagem.
2.  **Pré-processamento:** Se desejar, ative as opções de pré-processamento (escala de cinza, binarização) utilizando os checkboxes.
3.  **Executar OCR:** Clique no botão "Executar OCR" para extrair o texto da imagem.
4.  **Visualizar Resultados:** O texto extraído será exibido na área de texto e a imagem pré-processada (se aplicável) será exibida na interface.

## Estrutura do Projeto

O projeto é composto por dois arquivos principais:

* **`OcrApp.h`:** Arquivo de cabeçalho que declara a classe `OCRApp` e seus membros.
* **`OcrApp.cpp`:** Arquivo de implementação que define os métodos da classe `OCRApp`.

## Melhorias Futuras

* Adicionar mais opções de pré-processamento (rotação, correção de distorção, etc.).
* Implementar funcionalidades de edição e correção do texto extraído.
* Permitir a seleção de diferentes idiomas para o OCR.
* Melhorar a interface gráfica para uma melhor experiência do usuário.
* Otimizar o desempenho do OCR para imagens de alta resolução.

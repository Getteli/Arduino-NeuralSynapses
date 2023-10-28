/**
  * Captura de sinais eletricos via eletrodos. Para o TCC - Unicarioca 2023.2
  * Arduino: mega 2560
  * Autor: Douglas Araujo Silva
  * Data: 17/10/2023
  * Observação: Este código foi criado e calibrado pelo Douglas para o seu TCC sobre: O USO DAS SINAPSES NEURAIS NA COMPUTAÇÃO
*/

/* DECLARAÇÃO DE VÁRIAVEIS */
	int   pinoEletrodo        = A15;	// Pino analógico onde o eletrodo está conectado (vermelho capta) (preto terra)
	int   ledSinal            = 7;		// indica a porta em que o led está conectado
	float reducaoTensao       = 0.5;	// Fator de redução de tensão em % (1 = 100%)
	float VoltagemReferencial = 5;		// valor de referencia da voltagem com que o arduino mega AT2560 trabalha (4.5v ~ 5.5v)
	int   valorOnda           = 1023;	// valor analógico em quantidade de BITS, com que o mega AT2560 trabalha com 10 bits (2^N)
	int   valorAnalogico      = 0;		// váriavel que armazenará o valor elétrico lido
	float tensaoLida          = 0;		// váriavel que armazenará o resultado da formula de tensão
	int   casasDecimais       = 3;		// quantidade de casas decimais para a tensão lida
	unsigned long tmpLeitura  = 20;		// tempo para registro da leitura em microsegundo
	bool lerTensao            = false;	// variavel para ler a tensao (onda modular, captando sinais eletricos) ou ler em binário

	unsigned long tmpbinario  = 20;		// tempo para registro da leitura em microsegundo
	String binario            = "";		// armazena o vario do binário completo para ser convertido e exibido
	int binarioConvertido     = 0;		// valor que armazenará o binário convertido para decimal, atual.
	int faixaEstavel          = 130;	// valor para calibragem, sobre a faixa em que o binario convertido esta em estado normal, antes de alguma ação
	bool lerBinario           = true;	// variavel para saber se vai ler apenas o numero convertido ou o osciloscopio p/ numero inteiro

/* INIT */
	void setup()
	{
		Serial.begin(9600); // Inicializa a comunicação serial
		analogReference(DEFAULT); // Configura a referência analógica para 5V
		pinMode(ledSinal, OUTPUT);
	}

/* LOOP*/
	void loop()
	{
		// Lê o valor analógico do pino
		valorAnalogico = analogRead(pinoEletrodo);

		if(lerTensao)
		{
			/**
			* cálculo de calibragem para compensar, evitar interferencias e conseguir a amostragem o mais proximo do sinal correto
			*
			* fórmula: (Sinal * V) / 2^N * RT
			*/
			tensaoLida = (valorAnalogico * VoltagemReferencial) / valorOnda * reducaoTensao;

			/* EXIBE O VALOR*/
			Serial.println(tensaoLida,casasDecimais);

			// /*ACENDE O LED PELA AÇÃO*/
			if(tensaoLida > 0)
			{
				// acende o led e deixa um tempo de 2 ações
				digitalWrite(ledSinal, HIGH);
				delay(20);
				// apaga o led
				digitalWrite(ledSinal, LOW);
			}

			/* TEMPO PARA CAPTAÇÃO E LEITURA */
			delay(tmpLeitura);
		}
		else
		{
			/* EXIBIÇÃO E CONVERSAO DO SINAL ELETRICO EM BINÁRIO*/
			for (int i = 9; i >= 0; i--)
			{
				// armazena o binario na string
				binario += bitRead(valorAnalogico, i);

				if(lerBinario)
				{
					Serial.print(bitRead(valorAnalogico, i));
				}
			}

			// converte o binario completo para um numero em decimal
			binarioConvertido = strtol(binario.c_str(), NULL, 2);

			/* EXIBIÇÃO - verifica se deseja ler o binário ou apenas o decimal */
			if(lerBinario)
			{
				Serial.print("\n------- binario lido: ");
				Serial.print(binarioConvertido); // exibe o valor convertido do binário
				Serial.print(" ---------\n");
			}
			else
			{
				Serial.println(binarioConvertido);
			}

			/*ACENDE O LED PELA AÇÃO*/
			if(binarioConvertido >= faixaEstavel)
			{
				// acende o led e deixa um tempo de 2 ações
				digitalWrite(ledSinal, HIGH);

				delay(20);

				// apaga o led
				digitalWrite(ledSinal, LOW);

				// exit(0); // para o código para ler no terminal o sinal captado
			}

			// limpa a variavel do binario
			binario = "";

			/* TEMPO MAIOR PARA A LEITURA EM BINARIO*/
			delay(tmpbinario);
		}
	}
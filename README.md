# cinta_transportadora
conveyor belt arduino completed
/*
 * Proyecto: Cinta transportadora con llenadora de líquidos
 * Componentes:
 * - 2 módulos L298N (motor DC y bomba de agua)no descartar uso de capacitores
 * - 2 sensores HW201 (infrarrojos)
 * - LCD I2C 16x2: Conectar SDA a A4, SCL a A5 Alimentación 5V y GND
 * - LEDs: 
     LED Verde (D12) con resistencia 220Ω en serie
     LED Rojo (D11) con resistencia 220Ω en serie
 * - Pulsador: Conectar un extremo a D7 y otro a GND
 * - Resistencia pull-up interna activada en código
 
 * Funcionamiento:
 * 1. Motor DC funciona hasta que Sensor1 detecta objeto
 * 2. Se detiene motor DC, espera 3 segundos
 * 3. Activa bomba de agua por 4 segundos
 * 4. Se detiene bomba, espera 2 segundos
 * 5. Reactiva motor DC hasta que Sensor2 detecta el objeto
 * 6. Sensado de Nivel: Usa un sensor ultrasónico (HC-SR04) para medir la altura del líquido.
 * 7. Comunicación Inalámbrica: Módulo Bluetooth (HC-05) para control remoto desde tu celular.
 * 8. Registro de Datos: Guarda eventos en una tarjeta SD con fechas/horas.
 * 9. Control por pulsador:
          - Pulsa el botón para pausar/reanudar todo el sistema
          - Los LEDs indican el estado actual
 *10. LCD muestra:
          - Línea 1: Estado de cinta y bomba
          - Línea 2: Contador de ciclos de llenado
 *11. Protecciones:
          - Debounce para el pulsador
          - Actualización eficiente del LCD

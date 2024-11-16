### Instalación
## Control automático de compuerta y monitoreo del entorno
### Pre requisitos
Deben estar instalados:

- VSCode
- Platformio
- ST-Link debugger
- Placa de desarrollo STM32F103CB

### Descargar repositorio

`git clone https://github.com/ICOMP-UNC/N-meros-magicos.git`

Abrir proyecto en VSCode-platformio. El archivo de configuración de proyecto se encuentra en `N-meros-magicos/platformio.ini`.

Para compilar el proyecto desde platformIO Core CLI:
`pio run`

Para subir el firmware a la placa desde platformIO Core CLI:
`pio run -t upload`
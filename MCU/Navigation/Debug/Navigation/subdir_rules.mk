################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Navigation/KalmanFilter.obj: ../Navigation/KalmanFilter.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/KalmanFilter.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/MPU9250.obj: ../Navigation/MPU9250.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/MPU9250.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/MadgwickAHRS.obj: ../Navigation/MadgwickAHRS.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/MadgwickAHRS.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/gps.obj: ../Navigation/gps.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/gps.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/i2c1.obj: ../Navigation/i2c1.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/i2c1.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/main.obj: ../Navigation/main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/main.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/timer.obj: ../Navigation/timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/timer.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/tm4c123gh6pm_startup_ccs.obj: ../Navigation/tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/tm4c123gh6pm_startup_ccs.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

Navigation/uartout.obj: ../Navigation/uartout.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/mairon/ti/TivaWare_C_Series" --include_path="/home/mairon/Desktop/UFSC/TCC/Códigos/MCU/Navigation" --include_path="/home/mairon/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Navigation/uartout.d_raw" --obj_directory="Navigation" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '



#include "ATcommands.h"


/**

    @brief Sends a test command.
    This function formats the command as "%s=?" and sends it over the transmit channel.
    @param buffer The buffer to store the formatted command.
    @param tx The transmit function.
    @param cmd The command string.
    @return The length of the formatted command if successful, otherwise AT_CMD_ERROR.
	*/

int at_test_command(char *buffer, transmit_t tx, char *cmd) {
	int length;
	if((length = sprintf_(buffer, "%s=?\r\n", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}


/**

    @brief Sends a query command.
    This function formats the command as "%s?" and sends it over the transmit channel.
    @param buffer The buffer to store the formatted command.
    @param tx The transmit function.
    @param cmd The command string.
    @return The length of the formatted command if successful, otherwise AT_CMD_ERROR.
    */

int at_query_command(char *buffer, transmit_t tx, char *cmd) {
	int length;
	if((length = sprintf_(buffer, "%s?\r\n", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}


/**

    @brief Sends a set command with variable parameters.
    This function formats the command as "%s=<param_format>" and sends it over the transmit channel.
    The parameter format and additional parameters are specified using variable arguments.
    @param buffer The buffer to store the formatted command.
    @param tx The transmit function.
    @param cmd The command string.
    @param param_format The format string for the parameters.
    @param ... Additional parameters for the format string.
    @return The length of the formatted command if successful, otherwise AT_CMD_ERROR.
    */

int at_set_command(char *buffer, transmit_t tx, char *cmd, const char* param_format, ...) {

	va_list parameters;
	int length = 0;
	int tmp = 0;
	if((tmp = sprintf_(buffer, "%s=", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	tmp = 0;
	va_start(parameters, param_format);
	if((tmp = vsprintf_(buffer+length, param_format, parameters)) < 0) {
		return AT_CMD_ERROR;
	}
	va_end(parameters);
	length += tmp;
	tmp = 0;
	if((tmp = sprintf_(buffer+length, "\r\n")) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}


/**

    @brief Sends a set command with limited length.
    This function formats the command as "%s=<param_format>" and sends it over the transmit channel.
    The parameter format and additional parameters are specified using variable arguments.
    The length of the formatted command is limited to the specified limit.
    @param buffer The buffer to store the formatted command.
    @param tx The transmit function.
    @param limit The maximum length of the formatted command.
    @param cmd The command string.
    @param param_format The format string for the parameters.
    @return The length of the formatted command if successful, otherwise AT_CMD_ERROR.
    */

int at_setn_command(char *buffer, transmit_t tx, size_t limit, char *cmd, const char* param_format, ...) {
	va_list parameters;
	int length = 0;
	int tmp = 0;
	if((tmp = sprintf_(buffer, "%s=", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	tmp = 0;
	va_start(parameters, param_format);
	if((tmp = vsnprintf_(buffer+length, limit, param_format, parameters)) < 0) {
		return AT_CMD_ERROR;
	}
	va_end(parameters);
	length += tmp;
	tmp = 0;
	if((tmp = sprintf_(buffer+length, "\r\n")) < 0) {
		return AT_CMD_ERROR;
	}
	length += tmp;
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}


/**

    @brief Sends an execute command.
    This function formats the command as "%s" and sends it over the transmit channel.
    @param buffer The buffer to store the formatted command.
    @param tx The transmit function.
    @param cmd The command string.
    @return The length of the formatted command if successful, otherwise AT_CMD_ERROR.
    */

int at_execute_command(char *buffer, transmit_t tx, char *cmd) {
	int length;
	if((length = sprintf_(buffer, "%s\r\n", cmd)) < 0) {
		return AT_CMD_ERROR;
	}
	if(tx != NULL) {
		tx(buffer, length);
	}
	return length;
}

#include "pistaOS_event.h"

pistaOS_event_t event_init(void) {
	/* obtener recurso, no es mas que la posición en el array de tareas
	 *  de la tarea que ejecuta esta función. */
	pistaOS_event_t result;

	result.id = get_running_task_index();
	result.taken = 0;

	return result;
}

void event_wait(pistaOS_event_t* event_id) {

	// asociar el recurso a la tarea (el id del recurso ya coincide con el de la tarea)
	// poner la tarea en suspenso
	// llamar scheduler
	event_id->taken = 1;
	set_task_status(event_id->id, TASK_SUSPENDED);
}

void event_set(pistaOS_event_t* event_id) {

	// poner tarea asociada al recurso en ready
	// llamar a scheduler
	if(event_id->taken == 1){
		event_id->taken = 0;
		set_task_status(event_id->id, TASK_READY);
	}

}

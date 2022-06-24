/*
    getuserweight() should return the weight of the specified user, or -1 on error.

    The default user weight should be 10.

    setuserweight() should give the specified user the specified weight, and return 0, or -1 on error.

    For either call, a uid value of -1 indicates the current user (that is, the owner of the task that is making the system call). Otherwise, the range of valid uids is 0-65535. A bad uid should result in a return value of -1 and errno set to EINVAL.

    Only root is allowed to call setuserweight(); a call by any other user should return -1 and set errno to EACCES.

    Also, a user’s weight must be greater than zero. Any attempt to set a bad weight is an error, and errno should be set to EINVAL.

*/
#include <linux/syscalls.h>
#include <linux/user_namespace.h>

// inicia pesos com valor definido
int weights[65536] = {[0 ... 65535] = 10};

SYSCALL_DEFINE1(getuserweight, int, uid) {
	if(uid < -1 || uid > 65535){
		return  -EINVAL;
	}

	if(uid == -1){
		// avalia se estamos lidando com o usuario atual
		// caso seja o usuario atual vamos buscar o uid dele no sistema
		uid = from_kuid_munged(current_user_ns(), current_uid());
	}
	printk("uid requisitado: %d\n", uid);
	return weights[uid];
}

SYSCALL_DEFINE2(setuserweight, int, uid, int, weight) {
	if(from_kuid_munged(current_user_ns(), current_uid()) != 0) {
		// checa se estamos chamando com com privilegios sudo
		return -EACCES;
	}

	if(uid < -1 || uid > 65535 || weight <= 0) {  
		// checa se estamos dentro do valor permitido de uids
		return -EINVAL;
	}

	if(uid == -1){
		// trata o caso de estarmos lidando com o usuario atual
		uid = from_kuid_munged(current_user_ns(), current_uid());
	}
	weights[uid] = weight;
	return 0;
}

/* vi: noexpandtab tabstop=4 softtabstop=4 shiftwidth=4
 *
 * ckatsak, Wed 24 Aug 2022 02:45:00 AM EEST
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <fcntl.h>
#include <linux/ptp_clock.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

/* As found in linuxptp v1.8.1 */
#define CLOCKFD 3
#define FD_TO_CLOCKID(fd)	((~(clockid_t) (fd) << 3) | CLOCKFD)
#define CLOCKID_TO_FD(clk)	((unsigned int) ~((clk) >> 3))

#define DEFAULT_PATH_SIZE       64
#define DEFAULT_PTP_DEV_PATH    "/dev/ptp0"

static PyObject *
clock_to_sys(PyObject * Py_UNUSED(self), PyObject *args)
{
	char path[DEFAULT_PATH_SIZE] = DEFAULT_PTP_DEV_PATH;
	int clock_fd;
	struct ptp_clock_caps caps;
	clockid_t clkid;
	struct timespec now;
	int check_validity = 0;

	if (!PyArg_ParseTuple(args, "|sp", &path, &check_validity))
		// PyArg_ParseTuple sets an appropriate (generic) exception by default,
		// which may be overriden to be more specific. For now, let it be.
		return NULL;

	// Open the clock device at the provided path
	if ((clock_fd = open(path, O_RDONLY)) < 0)
		return PyErr_SetFromErrno(PyExc_OSError);
	clkid = FD_TO_CLOCKID(clock_fd);

	// Check if clkid is valid
	if (check_validity) {
		if (ioctl(CLOCKID_TO_FD(clkid), PTP_CLOCK_GETCAPS, &caps)) {
			PyErr_SetString(PyExc_RuntimeError, "Invalid clock id");
			return NULL;
		}
	}

	// Read from the provided clock
	if (clock_gettime(clkid, &now))
		return PyErr_SetFromErrno(PyExc_OSError);

	// Set the System clock
	if (clock_settime(CLOCK_REALTIME, &now))
		return PyErr_SetFromErrno(PyExc_OSError);

	close(clock_fd);

	Py_RETURN_NONE;
}

PyDoc_STRVAR(clock_to_sys_doc,
"clock_to_sys(\n"
"	path: str = \"" DEFAULT_PTP_DEV_PATH "\",\n"
"	check_validity: bool = False,\n"
") -> None\n\n"
"Read the time from the clock device at the provided `path`, and sync the\n"
"System clock according to this time.\n"
"If `check_validity` evaluates to True, an additional `ioctl` is issued, to\n"
"check the validity of the clockid_t.\n\n"
"WARNING: This is not meant to be a high-precision sync.");
static PyMethodDef ptpsync_Methods[] = {
    {"clock_to_sys", clock_to_sys, METH_VARARGS, clock_to_sys_doc},
    {NULL, NULL, 0, NULL}
};

PyDoc_STRVAR(ptpsync_module_doc,
"ptpsync is a module that provides the `clock_to_sys` function, which\n"
"synchronizes the System clock with the time read from the provided clock\n"
"device (which, in my case, is meant to be KVM's paravirtualized PTP clock).");
static struct PyModuleDef ptpsync_module = {
    PyModuleDef_HEAD_INIT,
    .m_name = "ptpsync",   /* name of module */
    .m_doc  = ptpsync_module_doc, /* module documentation, may be NULL */
    .m_size = -1,       /* size of per-interpreter state of the module,
                        or -1 if the module keeps state in global variables. */
    ptpsync_Methods
};

PyMODINIT_FUNC
PyInit_ptpsync(void)
{
    return PyModule_Create(&ptpsync_module);
}


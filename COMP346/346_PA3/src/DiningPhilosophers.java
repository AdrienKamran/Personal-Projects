/**
 * Class DiningPhilosophers The main starter.
 *
 * @author Serguei A. Mokhov, mokhov@cs.concordia.ca
 */
public class DiningPhilosophers {
	/*
	 * ------------ Data members ------------
	 */

	/**
	 * This default may be overridden from the command line
	 */
	public static final int DEFAULT_NUMBER_OF_PHILOSOPHERS = 4;

	/**
	 * Dining "iterations" per philosopher thread - during "socialization"
	 */
	public static final int DINING_STEPS = 10;

	/**
	 * Shared monitor for philosophers
	 */
	public static Monitor soMonitor = null;

	/*
	 * ------- Methods -------
	 */

	/**
	 * Main begins here
	 */
	public static void main(String[] argv) {
		try {

			// Default number of philosophers (assuming no command line arg)
			int iPhilosophers = DEFAULT_NUMBER_OF_PHILOSOPHERS;

			// If arg is passed to command line:
			if (argv.length != 0) {
				try {
					if (argv.length > 1){
						// Will not accept more than 1 value
						throw new IllegalArgumentException("More than 1 value provided");
					}

					iPhilosophers = Integer.parseInt(argv[0]);

					if (iPhilosophers < 0 || iPhilosophers > 5) {
						// Will not accept invalid number of philosophers
						throw new IllegalArgumentException("Invalid number of philosophers present");
					}
				} catch (NumberFormatException e) {
					reportException(e);
					System.exit(404);
				} catch (IllegalArgumentException e) {
					reportException(e);
					System.exit(404);
				}
			}

			// Feed number of philosophers to the monitor
			soMonitor = new Monitor(iPhilosophers);

			// Creating space for all the philosophers
			Philosopher aoPhilosophers[] = new Philosopher[iPhilosophers];

			System.out.println(iPhilosophers + " philosopher(s) came in for a dinner.");

			// Philosophers take their seats here
			for (int j = 0; j < iPhilosophers; j++) {
				aoPhilosophers[j] = new Philosopher();
				aoPhilosophers[j].start();
			}

			// Main waits until all philosophers have finished eating
			for (int j = 0; j < iPhilosophers; j++)
				aoPhilosophers[j].join();

			System.out.println("All philosophers have left. System terminates normally.");
		} catch (InterruptedException e) {
			System.err.println("main():");
			reportException(e);
			System.exit(1);
		}
	} // main()

	/**
	 * Outputs exception information to STDERR
	 * 
	 * @param poException Exception object to dump to STDERR
	 */
	public static void reportException(Exception poException) {
		System.err.println("Caught exception : " + poException.getClass().getName());
		System.err.println("Message          : " + poException.getMessage());
		System.err.println("Stack Trace      : ");
		poException.printStackTrace(System.err);
	}
}

// EOF

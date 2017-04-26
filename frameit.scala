// run mmt with command line argument "mbt frameit.scala" to produce the .omdoc 

//fixed parameters:
val path_to_archive = "/home/daniel/FrameIT/mmt/Theories"
val solution_theory = "http://cds.omdoc.org/FrameIT?solution_theory"
val view            = "http://cds.omdoc.org/FrameIT?situation_problem_view"

// actual workload:
/**controller.handleLine("log console")*/
controller.handleLine("extension info.kwarc.mmt.frameit.FrameitPlugin")
controller.handleLine("mathpath archive " + path_to_archive)

/**controller.handleLine("log console")*/
val frameit = controller.extman.get(classOf[info.kwarc.mmt.frameit.FrameitPlugin]).head

/**controller.handleLine("log console")
println(frameit.run(solution_theory,view))*/

info.kwarc.mmt.api.utils.File.write(info.kwarc.mmt.api.utils.File("/home/daniel/FrameIT/pushout.xml"), frameit.run(solution_theory,view))

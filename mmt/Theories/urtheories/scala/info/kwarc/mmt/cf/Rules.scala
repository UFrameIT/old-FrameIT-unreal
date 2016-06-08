package info.kwarc.mmt.cf

import info.kwarc.mmt.api._
import execution._
import checking._
import objects._

import info.kwarc.mmt.lf._

object Sequence extends ExecutionRule(CF.seq) {
  def apply(callback: ExecutionCallback, env: RuntimeEnvironment, prog: Term) = {
    prog match {
      case ComplexTerm(head, _, vars, steps) =>
        val varsE = vars map {case VarDecl(n, tp, df, nt) =>
          val vdE = VarDecl(n, tp map callback.execute, df map callback.execute, nt)
          env.stack.newVariable(vdE)
          vdE
        }
        var result: Term = null
        steps map {a =>
          result = callback.execute(a)
        }
        env.stack.removeVariables(varsE.length)
        if (result == null)
          CF.noneT  // only if steps empty
        else
          result
    }
  }
}

object AssignmentTerm extends InferenceRule(CF.assign, OfType.path) {
   def apply(solver: Solver)(tm: Term, covered: Boolean)(implicit stack: objects.Stack, history: History) = {
     tm match {
       case OMA(headTerm, List(to, value)) =>
         val expTp = to match {
           case OMV(n) => stack.context(n).tp
           case OMS(p) => solver.getType(p)
           case _ =>
             solver.error("assignment must be to symbol or variable")
             None
         }
         expTp foreach {tp =>
           solver.check(Typing(stack, value, tp))
         }
         Some(OMS(CF.None))
       case _ =>
         solver.error("wrong number of arguments for assignment")
         None
     }
   }
}

object Assignment extends ExecutionRule(CF.assign) {
  def apply(callback: ExecutionCallback, env: RuntimeEnvironment, prog: Term) = {
    prog match {
      case OMA(headTerm, List(to, value)) =>
        val valueE = callback.execute(value)
        to match {
          case OMV(v) =>
            env.stack.assign(v, valueE)
          case OMS(field) =>
            if (field.module == env.heap.path) {
               val instDecl = env.heap.getInstance(field.name)
               instDecl.assign(field, valueE)
            } else {
              throw ExecutionError("cannot assign to non-instance: " + field.module)
            }
          case _ =>
            throw ExecutionError("cannot assign to non-identifier: " + to)
        }
    }
    CF.noneT
  }
}

object NewInstance extends ExecutionRule(CF.nw) {
  def apply(callback: ExecutionCallback, env: RuntimeEnvironment, prog: Term) = {
    prog match {
      case OMA(headTerm, List(of)) =>
        val i = env.heap.newInstance(env.stack.top, of)
        OMS(i.path)
    }
  }
}

object Print extends ExecutionRule(PL.print, List(Apply.path)) {
  def apply(callback: ExecutionCallback, env: RuntimeEnvironment, prog: Term) = {
    prog match {
      case Apply(headTerm, t) =>
        val tE = callback.execute(t)
        env.stdout.print(tE)
        CF.noneT
    }
  }
}


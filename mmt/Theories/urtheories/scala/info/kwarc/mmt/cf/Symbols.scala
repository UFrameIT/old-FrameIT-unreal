package info.kwarc.mmt.cf

import info.kwarc.mmt.api._
import objects._

object CF {
   val _base = info.kwarc.mmt.lf.Typed._base
   val _path = _base ? "CF"
   
   val nw = _path ? "new"
   val seq = _path ? "sequence"
   val assign = _path ? "assign"
   
   val None = _path ? "None"
   val none = _path ? "none"
   val noneT = OMS(none)
}

object PL {
   val _base = DPath(utils.URI("http", "cds.omdoc.org") / "examples")
   val _path = _base ? "ProgLang"
   
   val print = _path ? "print"
}
(* Frama-C journal generated at 13:47 the 25/09/2020 *)

exception Unreachable
exception Exception of string

[@@@ warning "-26"]

(* Run the user commands *)
let run () =
  Dynamic.Parameter.Bool.set "-eva" true;
  Dynamic.Parameter.Int.set "-eva-slevel" 10;
  Dynamic.Parameter.String.set "" "src/bin/entab.c";
  File.init_from_cmdline ();
  !Db.Value.compute ();
  Project.set_keep_current false;
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  let __ = Callgraph.Cg.get () in
  ()

(* Main *)
let main () =
  Journal.keep_file "./.frama-c/frama_c_journal.ml";
  try run ()
  with
  | Unreachable -> Kernel.fatal "Journal reaches an assumed dead code" 
  | Exception s -> Kernel.log "Journal re-raised the exception %S" s
  | exn ->
    Kernel.fatal
      "Journal raised an unexpected exception: %s"
      (Printexc.to_string exn)

(* Registering *)
let main : unit -> unit =
  Dynamic.register
    ~plugin:"Frama_c_journal.ml"
    "main"
    (Datatype.func Datatype.unit Datatype.unit)
    ~journalize:false
    main

(* Hooking *)
let () = Cmdline.run_after_loading_stage main; Cmdline.is_going_to_load ()

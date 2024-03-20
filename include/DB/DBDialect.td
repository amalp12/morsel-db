#ifndef DB_DIALECT
#define DB_DIALECT

#include "mlir/IR/OpBase.td"

def DB_Dialect : Dialect {
    let name = "db";
    let summary = "MLIR dialect to handle db queries";
	let description = [{
	Experimental Dialect
}];
	let cppNamespace = "::db";
	let hasConstantMaterializer = 1;
}

def DB_StringType:
    DialectType<DB_Dialect, CPred<"$_self.isa<StringRef>()">, "DB String Type">;

def DB_IntegerType: DB_Type<"Integer", "int"> {
  let summary = "Integer type with arbitrary precision up to a fixed limit";
  let description = [{
    Integer types have a designated bit width.
  }];
  /// Here we defined a single parameter for the type, which is the bitwidth.
  let parameters = (ins "unsigned":$width);

  /// Here we define the textual format of the type declaratively, which will
  /// automatically generate parser and printer logic. This will allow for
  /// instances of the type to be output as, for example:
  ///
  ///    !my.int<10> // a 10-bit integer.
  ///
  let assemblyFormat = "`<` $width `>`";

  /// Indicate that our type will add additional verification to the parameters.
  let genVerifyDecl = 1;
}

#endif
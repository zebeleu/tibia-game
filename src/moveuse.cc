#include "operate.hh"

// NOTE(fusion): Oh no.
void ChangeObject(Object Obj, ObjectType NewType, uint32 Value){
	if(!Obj.exists()){
		error("ChangeObject: Übergebenes Objekt existiert nicht (1, NewType=%d).\n", NewType.TypeID);
		return;
	}

	Object Con = Obj.getContainer();
	while(true){
		if(!Obj.exists()){
			error("ChangeObject: Übergebenes Objekt existiert nicht (2, NewType=%d).\n", NewType.TypeID);
			return;
		}

		try{
			// NOTE(fusion): `Change` requires CUMULATIVE objects to have AMOUNT
			// equal to one or it fails with `SPLITOBJECT`.
			ObjectType ObjType = Obj.getObjectType();
			if(ObjType.getFlag(CUMULATIVE)){
				uint32 Amount = Obj.getAttribute(AMOUNT);
				if(Amount > 1){
					Move(0, Obj, Con, Amount - 1, true, NONE);
				}
			}

			Change(Obj, NewType, Value);
			return;
		}catch(RESULT r){
			if(!Obj.exists()){
				error("ChangeObject: Übergebenes Objekt existiert nicht (3, NewType=%d).\n", NewType.TypeID);
				return;
			}

			if(r == CONTAINERFULL){
				Con = GetMapContainer(Obj);
			}else if(r == TOOHEAVY){
				Object MapCon = GetMapContainer(Obj);
				Move(0, Obj, MapCon, -1, true, NONE);
			}else if(r == NOROOM){
				if(!Obj.getObjectType().getFlag(CUMULATIVE)
						|| Obj.getAttribute(AMOUNT) <= 1){
					throw;
				}

				// TODO(fusion): Why do we use `Con` with `GetMapContainer` here?
				uint32 Amount = Obj.getAttribute(AMOUNT);
				Object MapCon = GetMapContainer(Con);
				Move(0, Obj, MapCon, Amount - 1, true, NONE);
			}else{
				throw;
			}
		}
	}
}

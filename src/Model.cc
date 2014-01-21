

#include "Model.h"



namespace jet{
namespace modeler{

    using ::jet::Exception;
    using namespace std;

    Model::Model( Utf8String name )
        :name(name)
    {


    }

    Model::~Model(){


    }

    void Model::setName( Utf8String name ){

        this->name = name;

    }

    Utf8String Model::getName() const{

        return this->name;

    }


    void Model::addField( ModelField *model_field ){

        ModelFieldMap::iterator it;
        Utf8String field_name = model_field->getName();

        if( field_name.isEmpty() ){
            throw new Exception( "Cannot add a field with an empty field name." );
        }

        it = this->fields.end();
        this->fields.insert( it, ModelFieldMapPair( field_name, model_field ) );

    }



    ModelField* Model::getFieldByName( Utf8String const &field_name ) const{

        ModelFieldMap::const_iterator it = this->fields.find( field_name );

        if( it == this->fields.end() ){
            return NULL;
        }

        return it->second;

    }



    std::ostream& Model::writeBaseModelFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;
        bool is_user_type;

        output_stream << endl <<
        endl <<
        "Base" << this->name << " = function(){" << endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = 'Base" << this->name << "';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        "    if( !this._collection_name ){" << endl <<
        "        this._collection_name = '" << this->name << "';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    MeteorModel.apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        endl;


        output_stream << endl << endl << endl;

        // call to initialization function
            output_stream <<
            "    me._Base" << this->name << " = function(){" << endl <<
            endl <<
            "        me._MeteorModel.apply( this, arguments );" << endl <<
            endl <<
            "    };"
            << endl << endl << endl;


        // the getter, setter and deleter for each of the fields
            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                if( model_field->isUserType() ){

                    output_stream <<

                    "    me.get" << model_field->getName().toCamelCase() << " = function(){" << endl <<
                    endl <<
                    "        if( !me._data." << model_field->getName() << "_id ){" << endl <<
                    "            return null;" << endl <<
                    "        }" << endl <<
                    endl <<
                    endl <<
                    "        try {" << endl <<
                    "            var result = new " << model_field->getType() << "().hydrateFromId( me._data." << model_field->getName() << "_id );" << endl <<
                    "        } catch( e ){" << endl <<
                    "           return null;" << endl <<
                    "        }" << endl <<
                    endl <<
                    endl <<
                    "        return result;" << endl <<
                    endl <<
                    "    };" << endl <<

                    endl <<
                    endl <<
                    endl <<


                    "    me.get" << model_field->getName().toCamelCase() << "Id = function(){" << endl <<
                    endl <<
                    "        if( !me._data." << model_field->getName() << "_id ){" << endl <<
                    "            return null;" << endl <<
                    "        }" << endl <<
                    endl <<
                    endl <<
                    "        return me._data." << model_field->getName() << "_id;" << endl <<
                    endl <<
                    "    };" << endl <<

                    endl <<
                    endl <<
                    endl <<

                    "    me.set" << model_field->getName().toCamelCase() << "Id = function( " << model_field->getName() << "_id ){ " << endl <<
                    endl <<
                    "        me.set( '" << model_field->getName() << "_id', " << model_field->getName() << "_id );" << endl <<
                    endl <<
                    "        return me;" << endl <<
                    endl <<
                    "    };" << endl <<
                    endl <<
                    endl <<



                    "    me.set" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){ " << endl <<
                    endl <<
                    "        me.set( '" << model_field->getName() << "_id', " << model_field->getName() << ".getId() );" << endl <<
                    endl <<
                    "        return me;" << endl <<
                    endl <<
                    "    };" << endl <<
                    endl <<
                    endl <<
                    endl;


                } else {

                    if( model_field->getType() == "array" ){

                        output_stream <<

                        "    me.add" << model_field->getName().toCamelCase() << " = function( value ){" << endl <<
                        endl <<
                        "        value = String(value);" << endl <<
                        endl <<
                        "        if( !me._data." << model_field->getName() << " || !Array.isArray(me._data." << model_field->getName() << ") ){" << endl <<
                        "            me._data." << model_field->getName() << " = [];" << endl <<
                        "        }" << endl <<
                        endl <<
                        "        if(_.indexOf(me._data." << model_field->getName() << ", value) == -1 ){" << endl <<
                        "            me._data." << model_field->getName() << ".push( value );" << endl <<
                        "            me.set( '" << model_field->getName() << "', me._data." << model_field->getName() << " );" << endl <<
                        "        }" << endl <<
                        endl <<
                        "        return me;" << endl <<
                        endl <<
                        "    };" << endl <<

                        endl <<
                        endl <<
                        endl <<

                        "    me.remove" << model_field->getName().toCamelCase() << " = function( value ){" << endl <<
                        endl <<
                        "        value = String(value);" << endl <<
                        endl <<
                        "        if( !me._data." << model_field->getName() << " || !Array.isArray(me._data." << model_field->getName() << ") ){" << endl <<
                        "            return false;" << endl <<
                        "        }" << endl <<
                        endl <<
                        "        _.remove( me._data." << model_field->getName() << ", function(item){" << endl <<
                        "            return item === value;" << endl <<
                        "        });" << endl <<
                        endl <<
                        "        return me;" << endl <<
                        endl <<
                        "    };" << endl <<

                        endl <<
                        endl <<
                        endl <<

                        "    me.get" << model_field->getName().toCamelCase() << "Array = function(){" << endl <<
                        endl <<
                        "        if( !me._data." << model_field->getName() << " || !Array.isArray(me._data." << model_field->getName() << ") ){" << endl <<
                        "            return [];" << endl <<
                        "        }" << endl <<
                        endl <<
                        "        return me._data." << model_field->getName() << ";" << endl <<
                        endl <<
                        "    };" << endl <<

                        endl <<
                        endl <<
                        endl <<

                        "    me.has" << model_field->getName().toCamelCase() << " = function( value ){" << endl <<
                        endl <<
                        "        if( !me._data." << model_field->getName() << " || !Array.isArray(me._data." << model_field->getName() << ") ){" << endl <<
                        "            me._data." << model_field->getName() << " = [];" << endl <<
                        "        }" << endl <<
                        endl <<
                        "        return _.indexOf( me._data." << model_field->getName() << ", value) != -1;" << endl <<
                        endl <<
                        "        };" << endl <<
                        endl <<
                        endl;

                    } else {

                        output_stream <<

                        "    me.get" << model_field->getName().toCamelCase() << " = function(){" << endl <<
                        endl <<
                        "        return String(me._data." << model_field->getName() << ").length ? me._data." << model_field->getName() << " : null;" << endl <<
                        endl <<
                        "    };" << endl <<

                        endl <<
                        endl <<
                        endl <<

                        "    me.set" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){ " << endl <<
                        endl <<
                        "        me.set( '" << model_field->getName() << "', " << model_field->getName() << " );" << endl <<
                        endl <<
                        "        return me;" << endl <<
                        endl <<
                        "    };" << endl <<
                        endl <<
                        endl <<
                        endl;

                    }

                }

            }


        output_stream <<
        "    if( me._class_name == 'Base" << this->name << "' ){" << endl <<
        "        me._Base" << this->name << "();" << endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;

        return output_stream;

    }


    std::ostream& Model::writeModelFile( std::ostream &output_stream ) const{

        output_stream << endl <<
        endl <<
        this->name << " = function(){" << endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = '" << this->name << "';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    Base" << this->name << ".apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        endl << endl << endl;


        output_stream <<
        "    me._" << this->name << " = function(){" << endl <<
        endl <<
        "        me._Base" << this->name << ".apply( this, arguments );" << endl <<
        endl <<
        "    };"
        << endl << endl << endl;


        output_stream << endl <<
        "    if( me._class_name == '" << this->name << "' ){" << endl <<
        "        me._" << this->name << "();" << endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;


        return output_stream;

    }






    std::ostream& Model::writeBaseListFile( std::ostream &output_stream ) const{

        ModelFieldMap::const_iterator it;
        ModelField *model_field;
        bool is_user_type;

        output_stream << endl <<
        endl <<
        "Base" << this->name << "List = function( data_reference ){" << endl <<
        endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = 'Base" << this->name << "List';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    XmlModel.apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        "    var list_members = [];" << endl <<
        endl <<
        endl <<
        endl <<
        "    me._list_node_name = '" << this->name.toUnderscoreCase() << "';" << endl <<
        endl <<
        endl <<
        endl <<
        "    me._Base" << this->name << "List = function(){" << endl <<
        "        me._XmlModel.apply( this, arguments );" << endl <<
        endl <<
        "        var list_children = me.getChildrenByTag( me._list_node_name );" << endl <<
        endl <<
        "        for( var i in list_children ){" << endl <<
        "            list_members.push( new " << this->name << "(list_children[i]) );" << endl <<
        "        }" << endl <<
        "    };" << endl;


        output_stream << endl << endl << endl <<
        "    me.getAll = function(){" << endl <<
        endl <<
        "        return list_members;" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<


        "    me.prepend = function( " << this->name.toLowerCase() << " ){" << endl <<
        endl <<
        "        list_members.unshift( " << this->name.toLowerCase() << " );" << endl <<
        "        return me;" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<



        "    me.append = function( " << this->name.toLowerCase() << " ){" << endl <<
        endl <<
        "        list_members.push( " << this->name.toLowerCase() << " );" << endl <<
        "        return me;" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<



        "    me.getByIndex = function( index ){" << endl <<
        endl <<
        "        if( index >= list_members.length || index < 0 ){" << endl <<
        endl <<
        "            throw \"Out of bounds for getByIndex\";" << endl <<
        endl <<
        "        }" << endl <<
        endl <<
        "        return list_members[ index ];" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<



        "    /**" << endl <<
        "     * Calls \"callback\" once for each list member. It passes the member model as the first parameter" << endl <<
        "     * and its index as the second parameter." << endl <<
        "     * @param function callback( member, index )" << endl <<
        "     * @returns void" << endl <<
        "     */" << endl <<
        "    me.forEach = function( callback ){" << endl <<
        endl <<
        "        if( typeof(callback) != 'function' ){" << endl <<
        "            return false;" << endl <<
        "        }" << endl <<
        endl <<
        "        for( var i in list_members ){" << endl <<
        "            callback( list_members[i], i );" << endl <<
        "        }" << endl <<
        endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl;



        // find_by-like functions for each of the accessors

            for( it = this->fields.begin(); it != this->fields.end(); it++ ){

                model_field = it->second;

                //find_by methods don't make sense for user types
                    is_user_type = model_field->isUserType();
                    if( is_user_type ){
                        continue;
                    }

                output_stream <<


                "    me.findBy" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){" << endl <<
                endl <<
                "        var results = [];" << endl <<
                "        for( var i in list_children ){" << endl <<
                "            if( list_children[i].get" << model_field->getName().toCamelCase() << "() == " << model_field->getName() << " ){" << endl <<
                "                results.push( list_children[i] );" << endl <<
                "            }" << endl <<
                "        }" << endl <<
                "        return results;" << endl <<
                endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl <<


                "    me.removeBy" << model_field->getName().toCamelCase() << " = function( " << model_field->getName() << " ){" << endl <<
                endl <<
                "        var new_array = [];" << endl <<
                "        for( var i in list_children ){" << endl <<
                "            if( list_children[i].get" << model_field->getName().toCamelCase() << "() != " << model_field->getName() << " ){" << endl <<
                "                new_array.push( list_children[i] );" << endl <<
                "            }" << endl <<
                "        }" << endl <<
                "        list_children = new_array;" << endl <<
                "        return me;" << endl <<
                endl <<
                "    };" << endl <<
                endl <<
                endl <<
                endl;


            }




        output_stream << endl <<
        "    if( me._class_name == 'Base" << this->name << "List' ){" << endl <<
        "        me._Base" << this->name << "List();" <<  endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;

        return output_stream;

    }


    std::ostream& Model::writeListFile( std::ostream &output_stream ) const{

        output_stream << endl <<
        endl <<
        this->name << "List = function( data_reference ){" << endl <<
        endl <<
        endl <<
        "    if( !this._class_name ){" << endl <<
        "        this._class_name = '" << this->name << "List';" << endl <<
        "    }" << endl <<
        endl <<
        endl <<
        endl <<
        "    Base" << this->name << "List.apply( this, arguments );" << endl <<
        endl <<
        "    var me = this;" << endl <<
        endl <<
        endl <<
        endl <<
        "    me._" << this->name << "List = function(){" << endl <<
        "        me._Base" << this->name << "List.apply( this, arguments );" << endl <<
        "    };" << endl <<
        endl <<
        endl <<
        endl <<
        "    if( me._class_name == '" << this->name << "List' ){" << endl <<
        "        me._" << this->name << "List();" << endl <<
        "    }" << endl <<
        endl <<
        "};" << endl;

        return output_stream;

    }



    std::ostream& operator<<( std::ostream &output_stream, const Model &output_model ){

        output_stream << output_model.name << "{" << endl;
        ModelFieldMap::const_iterator it;
        ModelField *model_field;

        for( it = output_model.fields.begin(); it != output_model.fields.end(); it++ ){

            model_field = it->second;
            output_stream << "    " << *model_field << endl;

        }

        output_stream << "};" << endl << endl;

        return output_stream;

    }




}
}
